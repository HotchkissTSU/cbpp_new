#ifndef CBPP_CSTRING_H
#define CBPP_CSTRING_H

#include <stddef.h>
#include <uchar.h>

namespace cbpp {
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
}

#endif
