#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <future>
#include "Global/global.h"
#include "Global/object.h"

namespace HCommon {

using task = std::function<void()>;
enum taskPriority {
    level0,
    level1,
    level2,
};

class ThreadPoolPrivate;
class ThreadPool:public Object
{
public:
    DECLARE_PRIVATE(ThreadPool)
    ThreadPool();
    ~ThreadPool();

    static ThreadPool *globalInstance();
    void setMaxThreadNum(int threadNum);

    template<class F, class... Args>
    void startFunc(taskPriority level, F&& f, Args&&...args){
        if(!isRunning()){
            throw std::runtime_error("the thread Pool is stoped");
        }
        using retType = decltype (f(args...));
        auto task = std::make_shared<std::packaged_task<retType()>>(
             std::bind(std::forward<F>(f), std::forward<Args>(args)...)
         );
        addTask(level, task);
    }

    template<class F, class... Args>
    auto startFuncNeedReturn(taskPriority level,F &&f, Args&&... args)->std::future<decltype (f(args...))>{
        if(!isRunning()){
            throw std::runtime_error("the thread Pool is stoped");
        }
        using retType = decltype (f(args...));
        auto task = std::make_shared<std::packaged_task<retType()>>(
             std::bind(std::forward<F>(f), std::forward<Args>(args)...)
         );
        addTask(level,task);
        return task->get_future();
    }

    int getIdleThreadNum();

private:
    void stopAll();
    bool isRunning();
    bool emplaceTask(taskPriority level, task task);

    template<class F>
    bool addTask(taskPriority &level,F &&task){
        return emplaceTask(level,[task]{
            (*task)();
        });
    }
};

}

#endif // THREADPOOL_H
