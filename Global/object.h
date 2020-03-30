#ifndef OBJECT_H
#define OBJECT_H
#include <memory>
#include "global.h"

namespace HCommon {

class ObjectData
{
public:
    DISABLE_COPY(ObjectData)
    ObjectData() = default;
    virtual ~ObjectData() {};
};

class Object
{
public:
    explicit Object(ObjectData &dd):d_ptr(&dd) {}
    virtual ~Object() {}

protected:
    std::shared_ptr<ObjectData> d_ptr;

};

}

#endif // OBJECT_H
