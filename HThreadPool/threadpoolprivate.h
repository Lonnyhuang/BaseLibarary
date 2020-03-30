#ifndef HYTHREADPOOLPRIVATE_H
#define HYTHREADPOOLPRIVATE_H
#include "threadpool.h"
#include <thread>
#include <vector>
#include <queue>
#include <atomic>
#include <string>
#include <condition_variable>
#include <map>

namespace HCommon {

class ThreadPoolPrivate:public ObjectData
{
public:
    explicit ThreadPoolPrivate();
    virtual ~ThreadPoolPrivate();
    void initThreadPool(int threadNum = 4);
    bool isRunning();
    void stop();
    bool emplaceTask(taskPriority level, task task);

private:
    void threadLoop();
public:
    std::atomic<int> m_idleNum;
private:
    typedef std::pair<taskPriority, task> TaskPair;
    struct TaskPriorityCmp{
        bool operator()(const TaskPair p1, const TaskPair p2){
            return p1.first > p2.first;
        }
    };
    std::mutex m_mutex;
    std::priority_queue<TaskPair,std::vector<TaskPair>, TaskPriorityCmp> m_tasks;
    std::vector<std::thread*> m_pool;
    std::condition_variable m_cv;
    std::atomic<bool> m_stop;
};

}

#endif // HYTHREADPOOLPRIVATE_H
