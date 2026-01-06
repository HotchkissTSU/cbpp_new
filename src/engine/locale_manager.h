#ifndef CBPP_LOCALE_MANAGER_H
#define CBPP_LOCALE_MANAGER_H

#include <stddef.h>

#include "cbpp/String.h"
#include "cbpp/Table.h"

// Which character to use as a mark that the string is a locale key
#define CBPP_LOCALE_KEY_MARK '#'

#define CBPP_LOCALE_FALLBACK "(NULL)"

namespace cbpp {    
    class CLocale {
        friend bool MountLocale(const char*);

        CBinTable<CString, CString> m_dKeys;

        public:
            CLocale() = default;
            CLocale(const CLocale& Other) = delete;
            CLocale(CLocale&& Other) = delete;

            CLocale& operator=(const CLocale& Other) = delete;
            CLocale& operator=(CLocale&& Other) = delete;

            const char* GetFallbackString() const;

            bool HasKey(const CString& sKey) const;
            const char* GetString(const CString& sKey) const;
    };

    class CLocaleManager {
        friend void SetLocale(const char*);
        friend const CLocale* GetCurrentLocale();

        CLocale* m_pCurrentLocale = NULL;
        CBinTable<CString, CLocale*> m_dLocales;

        public:
            
    };

    CLocaleManager* GetLocaleManager();

    bool MountLocale(const char* sFilePath);
    void SetLocale(const char* sLocaleName);

    const CLocale* GetCurrentLocale();
}

#endif
