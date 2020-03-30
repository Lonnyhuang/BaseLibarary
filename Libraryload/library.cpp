#include "library.h"
#include "libraryprivate.h"

HCommon::Library::Library():Object(*new LibraryPrivate())
{

}

HCommon::Library::~Library()
{

}

bool HCommon::Library::load(std::string name, bool ignoreError)
{
    return this->load(name, "", ignoreError);
}

bool HCommon::Library::load(std::string name, int verNum, bool ignoreError)
{
    return this->load(name, std::to_string(verNum), ignoreError);
}

bool HCommon::Library::load(std::string name, std::string version, bool ignoreError)
{
    D_PTR(Library)
    d->setFileNameAndVersion(name, version);

    bool re = d->load();
    if(!ignoreError){
        DefaultErrorHandler().OnLoadLibrary(name.c_str());
    }
    return re;
}

void HCommon::Library::Unload()
{
    D_PTR(Library)
    return d->Unload();
}

void *HCommon::Library::resolve(const std::string funcname, bool ignoreError)
{
    D_PTR(Library)
    void *ptr = d->resolve(funcname);
    if(!ignoreError){
        DefaultErrorHandler().OnLoadSymbol(funcname.c_str(), ignoreError);
        return ptr;
    }
    else
        return ptr;
}

bool HCommon::Library::isLoad()
{
    D_PTR(Library)
    return d->isLoad();
}

std::string HCommon::Library::fileName()
{
    D_PTR(Library)
    return d->fileName();
}

std::string HCommon::Library::version()
{
    D_PTR(Library)
    return d->version();
}
