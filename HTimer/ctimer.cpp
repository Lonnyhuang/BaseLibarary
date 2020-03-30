#include "timerprivate.h"

HCommon::CTimer::CTimer():Object(*new CTimerPrivate())
{

}

HCommon::CTimer::~CTimer()
{

}

void HCommon::CTimer::initTimer(int threadNum)
{
    CTimerPrivate::initTimer(threadNum);
}

void HCommon::CTimer::UnInitTimer()
{
    CTimerPrivate::UnInitTiemr();
}

HCommon::TimerId HCommon::CTimer::start(unsigned int intervalMsc, TimerFunc func, TIMERTYPE timertype)
{
    D_PTR(CTimer)
    return d->start(intervalMsc, func, timertype);
}

void HCommon::CTimer::cancel(HCommon::TimerId id)
{
    D_PTR(CTimer)
    return d->cancel(id);
}
