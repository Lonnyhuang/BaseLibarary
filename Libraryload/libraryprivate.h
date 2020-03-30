#ifndef LIBRARYPRIVATE_H
#define LIBRARYPRIVATE_H
#include "library.h"
#include "libraryException.h"

namespace HCommon {

struct DefaultErrorHandler
{
    template<class Char_>
    static void OnLoadLibrary(const Char_* libName)
    {
        throw LibraryNotFoundException(libName);
    }

    template<class Char_>
    static void OnLoadSymbol(const Char_* symbolName, const bool ignore)
    {
        if (!ignore) {
            throw SymbolNotFoundException(symbolName);
        }
    }
};

class LibraryPrivate:public ObjectData
{
public:
    explicit LibraryPrivate();
    virtual ~LibraryPrivate();
    bool load();
    void Unload();
    void *resolve(const std::string funcname);
    bool isLoad();
    void setFileNameAndVersion(const std::string &filename, const std::string &version);

    std::string fileName();
    std::string version();

private:
    using Libhandle = void*;
    Libhandle m_handle;
    std::string m_filename;
    std::string m_version;
};

}
#endif // LIBRARYPRIVATE_H
