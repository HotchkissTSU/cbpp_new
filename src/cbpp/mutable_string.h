#ifndef CBPP_MUTABLE_STRING_H
#define CBPP_MUTABLE_STRING_H

#include "cbpp/immutable_string.h"

namespace cbpp {
    /*
        Mutable heap-allocated null-terminated string
        with cached length
    */
    class CString : public CImmutableString {
        friend CString operator+(const CString& A, const CString& B);
        friend CString operator+(const CString& A, const char* B);

        protected:
            size_t m_iLength = 0;

        public:
            CString();
            CString(const char* sSource);
            CString(const CString& Other);

            char& operator[](size_t iIndex);
            char& At(size_t iIndex);

            char operator[](size_t iIndex) const;
            char At(size_t iIndex) const;

            // This string`s cached length
            size_t Length() const;

            // Recalculate the length and return it
            size_t LengthUpdate();

            CString& operator=(const CString& Other);
            CString& operator=(const char* sOther);

            CString& operator+=(const CString& Other);
            CString& operator+=(char iOther);

            operator char*();
            char* Pointer();
    };

    CString operator+(const CString& A, const CString& B);
    CString operator+(const CString& A, const char* B);
    //CString operator+(const CString& A, char B);
}

#endif
