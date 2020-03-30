#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <condition_variable>
#include "timerprivate.h"

namespace HCommon {

const int MAX_TIMERS = 2048;

class TimerManager
{
public:
    TimerManager();
    ~TimerManager();

    void initThread(int num);
    void unInitThread();
    int addTimer(TIMERINFO *timer);
    void cancel(int id);
private:
    void threadLoop();

    struct timerCmp{
        bool operator()(const TIMERINFO *p1, const TIMERINFO *p2){
            return p1->m_expires > p2->m_expires;
        }
    };
    std::priority_queue<TIMERINFO*, std::vector<TIMERINFO*>, timerCmp> m_queue;
    std::unordered_map<int, TIMERINFO*> m_timerMap;
    std::mutex m_mutex;
    std::vector<std::thread*> m_pool;
    std::condition_variable m_cv;
    std::atomic<bool> m_stop;
    int m_counter;
};
}
#endif // TIMERMANAGER_H
