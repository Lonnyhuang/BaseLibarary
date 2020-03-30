#include "timermanager.h"
#include "Global/ChronoTime.h"
#include <algorithm>

HCommon::TimerManager::TimerManager():m_stop(false),m_counter(0)
{

}

HCommon::TimerManager::~TimerManager()
{

}

void HCommon::TimerManager::initThread(int num)
{
    for(int i=0; i<num; i++){
        m_pool.emplace_back(new std::thread(std::bind(&TimerManager::threadLoop, this)));
    }
}

void HCommon::TimerManager::unInitThread()
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
    for(auto iter : m_timerMap){
        if(iter.second){
            delete iter.second;
            iter.second = nullptr;
        }
    }
    m_timerMap.clear();
    std::priority_queue<TIMERINFO*, vector<TIMERINFO*>, timerCmp> q;
    m_queue.swap(q);
}

int HCommon::TimerManager::addTimer(HCommon::TIMERINFO *timer)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if(m_queue.size() > MAX_TIMERS)
        return false;

    m_queue.push(timer);
    m_timerMap[++m_counter] = timer;
    return m_counter;
}

void HCommon::TimerManager::cancel(int id)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto iter = m_timerMap.find(id);
    if(iter != m_timerMap.end()){
        TIMERINFO *timer = iter->second;
        timer->m_cancel = true;
        m_timerMap.erase(id);
    }
}

void HCommon::TimerManager::threadLoop()
{
    while(!m_stop){
        TIMERINFO *timer;
        bool exec = false;
        {
            std::unique_lock<std::mutex> lock(this->m_mutex);
            m_cv.wait(lock,[this](){ return this->m_stop.load() || !this->m_queue.empty(); });
            if(!m_queue.empty() && !m_stop){
                timer = std::move(m_queue.top());
                if(timer->m_cancel){
                    m_queue.pop();
                    delete timer;
                }
                if(timer->m_expires <= ChronoTime::now_to_ms()){
                    exec = true;
                    m_queue.pop();
                }
            }
        }

        if(exec && timer->m_func){
            timer->m_func();
            if(timer->m_type == CircleShot){
                timer->m_expires = ChronoTime::now_to_ms() + timer->m_intervalMsc;
                addTimer(timer);
            }else{
                delete timer;
            }
        }
    }
}
