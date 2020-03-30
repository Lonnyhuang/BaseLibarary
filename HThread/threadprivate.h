#ifndef HYTHREADPRIVATE_H
#define HYTHREADPRIVATE_H
#include "thread.h"
#include <thread>
#include <mutex>

namespace HCommon {

class ThreadPrivate:public ObjectData
{
public:
    explicit ThreadPrivate();
    virtual ~ThreadPrivate();

    void setWorker(WorkerBase *);
    void setLoopMiliseconds(unsigned int miliseconds);
    bool isRunning();
    void start(PRIORITY pri);
    void stop();
    void detach();
    void work();
    std::thread::id getThreadId();
    int getThreadPriority();

private:
    void setThreadPriority(PRIORITY pri);
    void setThreadPriorityUnix();
    void setThreadPriorityWin();
    static void run(void *arg, int pri=0);

private:
    bool m_isRunning;
    WorkerBase *m_worker;
    unsigned int m_loopMiliseconds;
    std::thread m_thread;
    PRIORITY m_priority;
    std::mutex m_mutex;
};

}

#endif // HYTHREADPRIVATE_H
