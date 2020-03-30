#include "thread.h"
#include "threadprivate.h"

HCommon::Thread::Thread():Object(*new ThreadPrivate())
{

}

void HCommon::Thread::setWorker(WorkerBase *worker)
{
    D_PTR(Thread)
    d->setWorker(worker);
}

void HCommon::Thread::setLoopMiliseconds(unsigned int miliseconds)
{
    D_PTR(Thread)
    d->setLoopMiliseconds(miliseconds);
}

bool HCommon::Thread::isRunning()
{
    D_PTR(Thread)
    return d->isRunning();
}

void HCommon::Thread::start(HCommon::PRIORITY pri)
{
    D_PTR(Thread)
    return d->start(pri);
}

void HCommon::Thread::stop()
{
    D_PTR(Thread)
    return d->stop();
}

