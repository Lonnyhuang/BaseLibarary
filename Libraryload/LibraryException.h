#ifndef LIBRARYEXCEPTION_H
#define LIBRARYEXCEPTION_H
#include <exception>
#include <string>

class LibraryNotFoundException : public std::exception
{
public:
    LibraryNotFoundException(const char* err)
    {
        _err = err;
    }

    LibraryNotFoundException(const wchar_t* err)
    {
        static const size_t CONVERT_LEN = 256;
#if _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996)
#endif
        char mbsBuff[CONVERT_LEN + 1] = { 0 };
        std::wcstombs(mbsBuff, err, CONVERT_LEN);
        _err = mbsBuff;
#if _MSC_VER
#pragma warning(pop)
#endif
    }

    ~LibraryNotFoundException() throw() {}

    virtual const char* what() const throw() {
        return _err.c_str();
    }
private:
    std::string _err;
};

class SymbolNotFoundException : public std::exception
{
public:
    SymbolNotFoundException(const char* err)
    {
        _err = err;
    }

    SymbolNotFoundException(const wchar_t* err)
    {
        static const size_t CONVERT_LEN = 256;
#if _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996)
#endif
        char mbsBuff[CONVERT_LEN + 1] = { 0 };
        std::wcstombs(mbsBuff, err, CONVERT_LEN);
        _err = mbsBuff;
#if _MSC_VER
#pragma warning(pop)
#endif
    }

    ~SymbolNotFoundException() throw() { }

    virtual const char* what() const throw() {
        return _err.c_str();
    }

private:
    std::string _err;
};


#endif // LIBRARYEXCEPTION_H
