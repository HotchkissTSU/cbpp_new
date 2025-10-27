#ifndef CBPP_CONST_STRING_H
#define CBPP_CONST_STRING_H

#include <stddef.h>

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

    /*
        Get a new exactly copied string. Unlike strdup(), this call uses engine allocator
    */
    char* StringDup(const char* sSource, size_t* pLength = NULL);
}

#endif
