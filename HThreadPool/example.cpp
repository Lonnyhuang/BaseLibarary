#include "example.h"
#include "threadpool.h"
#include <algorithm>
#include <iostream>
std::mutex g_mutex;

void print(int i){
    std::cout << "task:" << i <<std::endl;
}

void testFunc(int i)
{
  std::lock_guard<std::mutex> lock(g_mutex);
  std::cout << "testFunc() [" << i << "] at thread [ " << std::endl;
}

int testFunc2(int i)
{
  std::lock_guard<std::mutex> lock(g_mutex);
  std::cout << "testFunc() [" << i << "] at thread [ " << std::endl;

  return i*2;

}

void exampleThreadPool()
{
    HCommon::ThreadPool pool;
    pool.setMaxThreadNum(4);
    for(int i = 0; i < 10 ; i++){
        pool.startFunc(HCommon::level0, testFunc, i);
    }

    std::future<int> future = pool.startFuncNeedReturn(HCommon::level0, testFunc2, 10);
    while(!future.valid()){}
    int re = future.get();
    std::cout << "testFunc2() [" << re << "] at thread [ " << std::endl;

    getchar();
}
