#include "threadpoolprivate.h"
#include <exception>
#include <iostream>

HCommon::ThreadPoolPrivate::ThreadPoolPrivate():m_stop(false)
{
}

HCommon::ThreadPoolPrivate::~ThreadPoolPrivate()
{
}

void HCommon::ThreadPoolPrivate::initThreadPool(int threadNum)
{
    m_idleNum = threadNum < 1 ? 1: threadNum;
    for(int i=0; i < m_idleNum; i++){
        m_pool.emplace_back(new std::thread(std::bind(&ThreadPoolPrivate::threadLoop, this)));
    }
}

bool HCommon::ThreadPoolPrivate::isRunning()
{
    return !m_stop.load();
}

void HCommon::ThreadPoolPrivate::stop()
{
    m_stop.store(true);
    m_cv.notify_all();
    for(std::thread *thread:m_pool){
        if(thread->joinable()){
            thread->join();
        }
        delete thread;
    }
    m_pool.clear();
}

bool HCommon::ThreadPoolPrivate::emplaceTask(taskPriority level, task task)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        TaskPair pair(level, task);
        m_tasks.push(pair);
    }
    m_cv.notify_one();
    return true;
}

void HCommon::ThreadPoolPrivate::threadLoop()
{
    while(!m_stop){
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(this->m_mutex);
            m_cv.wait(lock,[this](){ return this->m_stop.load() || !this->m_tasks.empty(); });
            if(!m_tasks.empty() && !m_stop){
                task = std::move(m_tasks.top().second);
                m_tasks.pop();
            }
        }
        --m_idleNum;
        if(task)
            task();
        ++m_idleNum;
    }
}
