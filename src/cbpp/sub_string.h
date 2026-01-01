#ifndef CBPP_REF_STRING_H
#define CBPP_REF_STRING_H

#include <stddef.h>

#include "cbpp/cbstring.h"

namespace cbpp {
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

            size_t Length() const;
            
            char* Start() const;
            char* End() const;

            CString Copy() const;

            // Output the string to an external buffer, adding the null-terminator
            size_t Bufferize(char* pBuffer, size_t iBuffSize) const;
    };
}

#endif
