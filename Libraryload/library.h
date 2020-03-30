#ifndef LIBRARY_H
#define LIBRARY_H
#include "Global/object.h"

namespace HCommon {

#define RESOLVE_OPTRIONAL(libraryPtr, funcType, funcname) \
    (funcType)libraryPtr->resolve(funcname);

class LibraryPrivate;
class Library:public Object
{
public:
    DECLARE_PRIVATE(Library)
    Library();
    ~Library();

    bool load(std::string name, bool ignoreError=true);
    bool load(std::string name, int verNum, bool ignoreError=true);
    bool load(std::string name, std::string version, bool ignoreError=true);
    void Unload();

    void *resolve(const std::string funcname, bool ignoreError=true);
    bool isLoad();
    std::string fileName();
    std::string version();
};

}
#endif // LIBRARY_H
