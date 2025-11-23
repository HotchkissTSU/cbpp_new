#ifndef CBPP_LOCALE_MANAGER_H
#define CBPP_LOCALE_MANAGER_H

#include <stddef.h>
#include <map>

#include "cbpp/immutable_string.h"

// Which character to use as a mark that the string is a locale key
#define CBPP_LOCALE_KEY_MARK '#'

#define CBPP_LOCALE_FALLBACK "(NULL)"

namespace cbpp {
    // Points inside current locale`s table
    class CLocaleString {
        friend class CLocale;

        CImmutableString* m_pData;

        CLocaleString(CImmutableString* pData) : m_pData(pData) {}

        public:
            CLocaleString() = delete;
            CLocaleString(const CLocaleString& Other) = delete;
            CLocaleString& operator=(const CLocaleString& Other) = delete;

            const char* String() const;
            operator const char* ();

            static const char* GetFallbackString();
    };
    
    class CLocale {
        typedef std::map<CImmutableString, CImmutableString> table_t;

        table_t m_dStrings;

        public:

    };
}

#endif
