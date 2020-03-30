#ifndef EXAMPLE_H
#define EXAMPLE_H
#include "ctimer.h"
#include <iostream>
#include <thread>

void func(){
    std::cout<<"time out"<<std::endl;
}

void exampleTimerTest(){
    HCommon::CTimer::initTimer(1);

    HCommon::CTimer timer;
    HCommon::TimerId id = timer.start(1000,std::bind(func), HCommon::CircleShot);

    std::this_thread::sleep_for(std::chrono::seconds(20));
    timer.cancel(id);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout<<"out"<<std::endl;
    HCommon::CTimer::UnInitTimer();
}
#endif // EXAMPLE_H
