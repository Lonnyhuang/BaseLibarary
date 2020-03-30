#include "timerprivate.h"

HCommon::Timer::Timer():Object(*new TimerPrivate())
{

}

HCommon::Timer::~Timer()
{

}

void HCommon::Timer::initTimer(int threadNum)
{
    TimerPrivate::initTimer(threadNum);
}

void HCommon::Timer::UnInitTimer()
{
    TimerPrivate::UnInitTiemr();
}

HCommon::TimerId HCommon::Timer::start(unsigned int intervalMsc, TimerFunc func, TIMERTYPE timertype)
{
    D_PTR(Timer)
    return d->start(intervalMsc, func, timertype);
}

void HCommon::Timer::cancel(HCommon::TimerId id)
{
    D_PTR(Timer)
    return d->cancel(id);
}
