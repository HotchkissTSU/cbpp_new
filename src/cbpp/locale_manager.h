#ifndef CBPP_LOCALE_MANAGER_H
#define CBPP_LOCALE_MANAGER_H

#include <stddef.h>
#include <map>

#include "cbpp/immutable_string.h"

// Which character to use as a mark that the string is a locale key
#define CBPP_LOCALE_KEY_MARK '#'

#define CBPP_LOCALE_FALLBACK "(NULL)"

namespace cbpp {
    class CLocale {
        CImmutableString m_sPrettyName;
        std::map<CImmutableString, CImmutableString> m_dEntries;

        public:
            CLocale(const char* sPrettyName);

            void PushString(const char* sKey, const char* sString);
            const char* GetString(const char* sKey) const;
            const char* PrettyName() const;
    };

    class CLocaleManager {
        friend const CLocaleManager* GetLocaleManager();

        CLocale* m_pCurrentLocale = NULL;
        const CImmutableString m_sDefaultFallback = CBPP_LOCALE_FALLBACK;

        std::map<CImmutableString, CLocale> m_dLocales;

        CLocaleManager() = default;

        public:
            const CLocale* CurrentLocale();
            void SetLocale(const char* sLocaleName);
            bool MountLocale(const char* sName);

            const char* GetString(const char* sKey) const;

            ~CLocaleManager();
    };

    const CLocaleManager* GetLocaleManager();
}

#endif
