#include "libraryprivate.h"
#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

HCommon::LibraryPrivate::LibraryPrivate():m_handle(nullptr)
{

}

HCommon::LibraryPrivate::~LibraryPrivate()
{
    if(isLoad()){
        Unload();
    }
}

bool HCommon::LibraryPrivate::load()
{
    if(isLoad()){
        Unload();
    }else if(m_filename.empty()){
        return false;
    }
#if defined(WIN32) || defined (WIN64)
    m_handle = LoadLibrary(reinterpret_cast<const wchar_t*>(m_filename.c_str()));
#else
    m_handle = dlopen(m_filename.c_str(), RTLD_LAZY);
#endif

    return m_handle != nullptr;
}

void HCommon::LibraryPrivate::Unload()
{
    if(!isLoad())
        return;
#if defined(WIN32) || defined (WIN64)
    FreeLibrary((HMODULE)m_handle);
#elif !defined(_ANDROID)
    dlclose(lib_handle);
#endif
    m_handle = NULL;
}

void *HCommon::LibraryPrivate::resolve(const std::string funcname)
{
    if(!isLoad())
        return nullptr;
#if defined(WIN32) || defined (WIN64)
    return (void *)GetProcAddress((HMODULE)m_handle, funcname.c_str());
#else
    return dlsym(lib_handle, fun_name.c_str());
#endif
}

bool HCommon::LibraryPrivate::isLoad()
{
    return m_handle != nullptr;
}

std::string HCommon::LibraryPrivate::fileName()
{
    return m_filename;
}

std::string HCommon::LibraryPrivate::version()
{
    return m_version;
}

void HCommon::LibraryPrivate::setFileNameAndVersion(const std::string &filename, const std::string &version)
{
    m_filename = filename;
    m_version = version;
}
