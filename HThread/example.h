#ifndef EXAMPLETHREAD_H
#define EXAMPLETHREAD_H
#include "workerbase.h"
#include "thread.h"
#include <iostream>
#include <thread>
#include <atomic>

class exampleWorker:public HCommon::WorkerBase
{
public:
    exampleWorker():m_finsh(false) {}
    virtual ~exampleWorker() {}

    virtual void doMain() override{
        int i = 0;
        while(!m_finsh){
            std::cout << "example :" << i++ << " " << m_finsh <<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "example done:" << i <<std::endl;
    }
    void finsh() {
        m_finsh = true;
    }
private:
    std::atomic<bool> m_finsh;
};

class exampleWorker2:public HCommon::WorkerBase
{
public:
    exampleWorker2():m_finsh(false) {}
    virtual ~exampleWorker2() {}

    virtual void doMain() override{
        int i = 0;
        while(!m_finsh){
            std::cout << "example2 :" << i++ << " " << m_finsh <<std::endl;
            //std::this_thread::sleep_for(std::chrono::seconds(1));
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "example2 done:" << i <<std::endl;
    }
    void finsh() {
        m_finsh = true;
    }
private:
    std::atomic<bool> m_finsh;
};

void exampleThread()
{
    HCommon::Thread thread;
    HCommon::Thread thread2;
    exampleWorker *worker = new exampleWorker;
    exampleWorker2 *worker2 = new exampleWorker2;
    thread.setWorker(worker);
    thread2.setWorker(worker2);
    thread.start(HCommon::LowestPriority);
    thread2.start(HCommon::HighestPriority);

    std::this_thread::sleep_for(std::chrono::seconds(7));
    worker->finsh();
    worker2->finsh();
    thread.stop();
    thread2.stop();
    //getchar();
}



#endif // EXAMPLETHREAD_H
