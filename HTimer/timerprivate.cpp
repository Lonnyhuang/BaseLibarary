#include "timerprivate.h"
#include "timermanager.h"

HCommon::TimerManager HCommon::CTimerPrivate::m_manager;

HCommon::CTimerPrivate::CTimerPrivate()
{

}

HCommon::CTimerPrivate::~CTimerPrivate()
{

}

void HCommon::CTimerPrivate::initTimer(int threadNum)
{
    m_manager.initThread(threadNum);
}

void HCommon::CTimerPrivate::UnInitTiemr()
{
    m_manager.unInitThread();
}

HCommon::TimerId HCommon::CTimerPrivate::start(unsigned int intervalMsc, HCommon::TimerFunc func, HCommon::TIMERTYPE timertype)
{
    TIMERINFO *timer = new TIMERINFO;
    timer->m_intervalMsc = intervalMsc;
    timer->m_type = timertype;
    timer->m_func = func;
    auto now = ChronoTime::now_to_ms();
    timer->m_expires = now + intervalMsc;
    return m_manager.addTimer(timer);
}

void HCommon::CTimerPrivate::cancel(HCommon::TimerId id)
{
    return m_manager.cancel(id);
}
