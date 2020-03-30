#include "threadpool.h"
#include "threadpoolprivate.h"

HCommon::ThreadPool::ThreadPool():Object(*new ThreadPoolPrivate())
{

}

HCommon::ThreadPool::~ThreadPool()
{
    stopAll();
}

HCommon::ThreadPool *HCommon::ThreadPool::globalInstance()
{
    static ThreadPool threadPool;
    return &threadPool;
}

void HCommon::ThreadPool::setMaxThreadNum(int threadNum)
{
    D_PTR(ThreadPool)
    return d->initThreadPool(threadNum);
}

int HCommon::ThreadPool::getIdleThreadNum()
{
    D_PTR(ThreadPool)
    return d->m_idleNum;
}

void HCommon::ThreadPool::stopAll()
{
    D_PTR(ThreadPool)
    return d->stop();
}

bool HCommon::ThreadPool::isRunning()
{
    D_PTR(ThreadPool)
    return d->isRunning();
}

bool HCommon::ThreadPool::emplaceTask(taskPriority level, task task)
{
    D_PTR(ThreadPool)
    return d->emplaceTask(level, task);
}
