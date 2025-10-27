#ifndef CBPP_IMMUT_H
#define CBPP_IMMUT_H

#include "cbpp/const_string.h"

namespace cbpp {
    /*
        A heap-allocated immutable string type
    */
    class CImmutableString {
        protected:
            char* m_sData;
        
        public:
            CImmutableString();
            CImmutableString(const char* sSource);
            CImmutableString(const CImmutableString& Other);

            bool operator<(const char* sOther) const;
            bool operator==(const char* sOther) const;
            bool operator>(const char* sOther) const;

            CImmutableString& operator=(const CImmutableString& Other);

            size_t Length() const;

            const char* String() const;
            operator cstring_t() const;

            operator bool() const;

            ~CImmutableString();
    };
}

#endif
