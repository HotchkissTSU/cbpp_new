#ifndef CBPP_STRING_API_H
#define CBPP_STRING_API_H

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdarg.h>

namespace cbpp {
    typedef const char* cstring_t;

    /*
        Constant compile-time string wrapper.
        Has operators overloaded and can be used in containers.
    */
    class CConstString {
        const char* m_sData;
        public:
            CConstString();
            CConstString(const char* sData);

            bool operator<(const char* sOther) const;
            bool operator==(const char* sOther) const;
            bool operator>(const char* sOther) const;

            size_t Length() const;

            const char* String() const;
            operator cstring_t() const;

            operator bool() const;
    };

    /*
        A regular string. Has it`s length cached, so if you
        have done something nasty with the data you must recache it
        with LengthUpdate() call, or something horrible might happen.

        Still NULL-terminated tho.
    */
    class CString {
        private:
            char* m_sData = NULL;
            size_t m_iLength = 0;

            void Nullify();
            
        public:
            CString();
            CString(size_t iLength);
            CString(const char* sSource);
            CString(const CString& Other);
            CString(CString&& Other);
            
            ~CString();
            
            CString& operator=(const char* sOther);
            CString& operator=(const CString& Other);
            CString& operator=(CString&& Other);
            
            const char* String() const;
            char* Pointer();
            
            size_t Length() const;
            size_t LengthUpdate();

            void Set(const CString& sData);
            void Set(const char* sData);

            // Resize string to host iLength characters
            void Resize(size_t iLength);
            
            bool IsValid() const;
            
            char& operator[](size_t iIndex);
            const char& operator[](size_t iIndex) const;
            char& At(size_t iIndex);
            const char& At(size_t iIndex) const;
            
            bool operator==(const char* sOther) const;
            bool operator==(const CString& Other) const;
            
            bool operator<(const char* sOther) const;
            bool operator<(const CString& Other) const;

            bool operator>(const char* sOther) const;
            bool operator>(const CString& Other) const;
            
            CString& operator+=(const CString& Other);
            CString& operator+=(char iOther);

            size_t Printv(const char* sFormat, va_list Args);
            size_t Printf(const char* sFormat, ...);
            
            operator cstring_t() const;
            operator char*();
            operator bool() const;
            
            friend CString operator+(const CString& A, const CString& B);
            friend CString operator+(const CString& A, const char* B);
    };

    CString operator+(const CString& A, const CString& B);
    CString operator+(const CString& A, const char* B);

    /*
        A reference that points to a substring in the source string.
        Useful when working with a single big text to avoid allocations and copying.

        Obviously, this class is useable until referenced string is valid.
    */
    class CSubString {
        char *m_pStart = NULL, *m_pEnd = NULL;

        public:
            CSubString(CString&& sSource, size_t iPos, size_t iLength) = delete;

            CSubString() = default;

            CSubString(const CString& sSource, size_t iStart, size_t iLength);
            CSubString(const char* sSource, size_t iStart, size_t iLength);

            void Set(const char* sSource, size_t iStart, size_t iLength);

            bool operator==(const CSubString& Other) const;
            bool operator<(const CSubString& Other) const;
            bool operator>(const CSubString& Other) const;

            size_t Length() const;
            
            char* Start() const;
            char* End() const;

            // Get substring as a new string
            CString Copy() const;

            // Output the string to an external buffer, adding the null-terminator
            size_t Bufferize(char* pBuffer, size_t iBuffSize) const;
    };

    /*
        Works like regular strcmp, but implemented for substrings
    */
    int32_t SubStringCmp(const CSubString& sA, const CSubString& sB);

    /*
        Get a new exactly copied string. Unlike strdup(), this call uses engine allocator
    */
    char* StringDup(const char* sSource, size_t* pLength = NULL);

    /* 
        Get current time formatted in a string buffer.
        Returns the amount of bytes written
    */
    size_t StringFormatTime(char* sBuffer, size_t iLength, const char* sFormat);

    /* 
        Get specified time formatted in a string buffer.
        Returns the amount of bytes written
    */
    size_t StringFormatTime(char* sBuffer, size_t iLength, const char* sFormat, time_t iTimer);

    /*
        -1 - not a number
        0  - integer
        1  - float 
    */
    int32_t IsNumber(const char* sString);
}

#endif
