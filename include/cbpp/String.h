#ifndef CBPP_STRING_API_H
#define CBPP_STRING_API_H

#include <stddef.h>
#include <stdint.h>

namespace cbpp {
    typedef const char* cstring_t;

    /*
        Constant compile-time string wrapper
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
            CString(CString&& Other) noexcept;
            
            ~CString();
            
            CString& operator=(const char* sOther);
            CString& operator=(const CString& Other);
            CString& operator=(CString&& Other) noexcept;
            
            const char* String() const;
            char* Pointer();
            
            size_t Length() const;
            size_t LengthUpdate();
            
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
            
            operator const char*() const;
            operator char*();
            operator bool() const;
            
            friend CString operator+(const CString& A, const CString& B);
            friend CString operator+(const CString& A, const char* B);
    };

    CString operator+(const CString& A, const CString& B);
    CString operator+(const CString& A, const char* B);

    /*
        A reference that points to a substring in the source string
        Useful when working with a single big text to avoid allocations and copying

        Obviously, this class is useable until referenced string is valid
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

            size_t Length() const;
            
            char* Start() const;
            char* End() const;

            CString Copy() const;

            // Output the string to an external buffer, adding the null-terminator
            size_t Bufferize(char* pBuffer, size_t iBuffSize) const;
    };

    int32_t SubStringCmp(const CSubString& sA, const CSubString& sB);
}

#endif
