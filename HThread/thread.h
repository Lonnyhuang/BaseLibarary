#ifndef HYTHREAD_H
#define HYTHREAD_H
#include "Global/global.h"
#include "Global/object.h"
#include "workerbase.h"

namespace HCommon {

enum THREADTYPE {
    STD_THREAD,
    QT_THREAD
};

enum PRIORITY{
    IdlePriority,  //空闲
    LowestPriority, //最低
    LowPriority, //低
    NormalPriority, //正常
    HighPriority, //高
    HighestPriority,  //最高
    TimeCriticalPriority,
    InheritPriority //继承
};

class ThreadPrivate;
class Thread:public Object
{
public:
    DECLARE_PRIVATE(Thread)
    Thread();
    ~Thread() {}

    void setWorker(WorkerBase *worker);
    void setLoopMiliseconds(unsigned int miliseconds);
    bool isRunning();
    void start(PRIORITY pri=NormalPriority);
    void stop();
};

}
#endif // HYTHREAD_H
