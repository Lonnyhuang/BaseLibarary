#ifndef TIMER_H
#define TIMER_H
#include "Global/object.h"

namespace HCommon {

using TimerFunc = std::function<void()>;
typedef int TimerId;

enum TIMERTYPE{
    SingleShot,
    CircleShot,
};

struct TIMERINFO{
    unsigned int m_intervalMsc;
    bool m_cancel;
    TIMERTYPE m_type;
    TimerFunc m_func;
    uint64_t m_expires;
    TIMERINFO(){
        m_intervalMsc = 0;
        m_cancel = false;
        m_type = SingleShot;
        m_func = nullptr;
        m_expires = 0;
    }
    TIMERINFO(const TIMERINFO &info){
        m_intervalMsc = info.m_intervalMsc;
        m_cancel = info.m_cancel;
        m_type = info.m_type;
        m_func = info.m_func;
        m_expires = info.m_expires;
    }
    TIMERINFO *operator=(const TIMERINFO &info){
        m_intervalMsc = info.m_intervalMsc;
        m_cancel = info.m_cancel;
        m_type = info.m_type;
        m_func = info.m_func;
        m_expires = info.m_expires;
        return this;
    }
};

class CTimerPrivate;
class CTimer:public Object
{
public:
    DECLARE_PRIVATE(CTimer)
    CTimer();
    ~CTimer();

    static void initTimer(int threadNum);
    static void UnInitTimer();
    TimerId start(unsigned int intervalMsc, TimerFunc func, TIMERTYPE timertype);
    void cancel(TimerId id);
};

}
#endif // TIMER_H
