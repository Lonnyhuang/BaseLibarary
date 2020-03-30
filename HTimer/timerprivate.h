#ifndef TIMERPRIVATE_H
#define TIMERPRIVATE_H
#include "ctimer.h"
//#include "timermanager.h"
#include "Global/ChronoTime.h"

namespace HCommon {

class TimerManager;
class CTimerPrivate:public ObjectData
{
public:
    explicit CTimerPrivate();
    virtual ~CTimerPrivate();

    static void initTimer(int threadNum);
    static void UnInitTiemr();
    TimerId start(unsigned int intervalMsc, TimerFunc func, TIMERTYPE timertype);
    void cancel(TimerId id);
private:
    static TimerManager m_manager;
};

}
#endif // TIMERPRIVATE_H
