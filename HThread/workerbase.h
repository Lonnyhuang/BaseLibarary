#ifndef WORKERBASE_H
#define WORKERBASE_H

namespace HCommon {

class WorkerBase
{
public:
    WorkerBase() = default;
    virtual ~WorkerBase() {};
    virtual void doMain() = 0;
};

}
#endif // WORKERBASE_H
