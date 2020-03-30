#include "threadprivate.h"
#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#else
#include <pthread.h>
#endif

static bool calculateUnixPriority(int priority, int *sched_policy, int *sched_priority)
{
#ifdef SCHED_IDLE
    if (priority == HCommon::IdlePriority) {
        *sched_policy = SCHED_IDLE;
        *sched_priority = 0;
        return true;
    }
    const int lowestPriority = HCommon::LowestPriority;
#else
    const int lowestPriority = HCommon::IdlePriority;
#endif
    const int highestPriority = HCommon::TimeCriticalPriority;
    int prio_min;
    int prio_max;
    {
    prio_min = sched_get_priority_min(*sched_policy);
    prio_max = sched_get_priority_max(*sched_policy);
    }
    if (prio_min == -1 || prio_max == -1)
        return false;
    int prio;
    // crudely scale our priority enum values to the prio_min/prio_max
    prio = ((priority - lowestPriority) * (prio_max - prio_min) / highestPriority) + prio_min;
    prio = HCommon::qMax(prio_min, HCommon::qMin(prio_max, prio));
    *sched_priority = prio;
    return true;
}

HCommon::ThreadPrivate::ThreadPrivate():m_isRunning(false),m_priority(IdlePriority)
{

}

HCommon::ThreadPrivate::~ThreadPrivate()
{
    if(m_worker){
        delete m_worker;
        m_worker = nullptr;
    }
}

void HCommon::ThreadPrivate::setWorker(WorkerBase *worker)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_worker = worker;
}

void HCommon::ThreadPrivate::setLoopMiliseconds(unsigned int miliseconds)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_loopMiliseconds = miliseconds;
}

bool HCommon::ThreadPrivate::isRunning()
{
    return m_isRunning;
}

void HCommon::ThreadPrivate::setThreadPriority(PRIORITY pri)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_priority = pri;
    //auto handle = m_thread.native_handle();
#if defined(WIN32) || defined (WIN64)
    return setThreadPriorityWin();
#else
    return setThreadPriorityUnix();
#endif
}

void HCommon::ThreadPrivate::start(PRIORITY pri)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_isRunning = true;
    m_thread = std::thread(&ThreadPrivate::run, this, pri);
    return;
}

void HCommon::ThreadPrivate::stop()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if(m_thread.joinable()){
        m_thread.join();
        m_isRunning = false;
    }
}

void HCommon::ThreadPrivate::detach()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if(m_thread.joinable()){
        m_thread.detach();
        m_isRunning = false;
    }
}

void HCommon::ThreadPrivate::work()
{
    if(m_worker)
        m_worker->doMain();
}

std::thread::id HCommon::ThreadPrivate::getThreadId()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_thread.get_id();
}

int HCommon::ThreadPrivate::getThreadPriority()
{
    return m_priority;
}

void HCommon::ThreadPrivate::setThreadPriorityUnix()
{
    int sched_policy;
    sched_param param;
    if (pthread_getschedparam(m_thread.native_handle(), &sched_policy, &param) != 0) {
        return;
    }
    int prio;
    if (!calculateUnixPriority(m_priority, &sched_policy, &prio)) {
        return;
    }
    param.sched_priority = prio;
    int status = pthread_setschedparam(m_thread.native_handle(), sched_policy, &param);

    if (status == -1 && sched_policy == SCHED_IDLE && errno == EINVAL) {
        // reset to lowest priority possible
        pthread_getschedparam(m_thread.native_handle(), &sched_policy, &param);
        param.sched_priority = sched_get_priority_min(sched_policy);
        pthread_setschedparam(m_thread.native_handle(), sched_policy, &param);
     }
}

void HCommon::ThreadPrivate::setThreadPriorityWin()
{
    int prio;
    switch (m_priority) {
    case HCommon::IdlePriority:
        prio = THREAD_PRIORITY_IDLE;
        break;
    case HCommon::LowestPriority:
        prio = THREAD_PRIORITY_LOWEST;
        break;
    case HCommon::LowPriority:
        prio = THREAD_PRIORITY_BELOW_NORMAL;
        break;
    case HCommon::NormalPriority:
        prio = THREAD_PRIORITY_NORMAL;
        break;
    case HCommon::HighPriority:
        prio = THREAD_PRIORITY_ABOVE_NORMAL;
        break;
    case HCommon::HighestPriority:
        prio = THREAD_PRIORITY_HIGHEST;
        break;
    case HCommon::TimeCriticalPriority:
        prio = THREAD_PRIORITY_TIME_CRITICAL;
        break;
    case HCommon::InheritPriority:
    default:
        return;
    }
    auto handle = m_thread.native_handle();
    if (!SetThreadPriority(&handle, prio)) {
        return;
    }
}

void HCommon::ThreadPrivate::run(void *arg, int pri)
{
    try {
        ThreadPrivate *thread = reinterpret_cast<ThreadPrivate *>(arg);
        thread->setThreadPriority(HCommon::PRIORITY(pri));
        thread->work();
    } catch (std::exception &e) {
        throw std::runtime_error("thread exec failed");
    }
}
