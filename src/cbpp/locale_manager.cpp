#include "cbpp/locale_manager.h"

#include <string.h>

#include "cbpp/malloc_wrapper.h"

namespace cbpp {
    CLocale::CLocale(const char* sPrettyName) : m_sPrettyName(sPrettyName) {}
    
    const char* CLocale::GetString(const char* sKey) const {
        if(m_dEntries.count(sKey) != 0) {
            return m_dEntries.at(sKey);
        }

        return NULL;
    }

    void CLocale::PushString(const char* sKey, const char* sString) {
        m_dEntries[sKey] = sString;
    }

    const char* CLocale::PrettyName() const { return m_sPrettyName; }

    const CLocaleManager* GetLocaleManager() {
        static CLocaleManager* s_pLocaleManager = new CLocaleManager();
        return (const CLocaleManager*)s_pLocaleManager;
    }

    const CLocale* CLocaleManager::CurrentLocale() {
        return (const CLocale*)m_pCurrentLocale;
    }

    void CLocaleManager::SetLocale(const char* sLocaleName) {
        if(m_pCurrentLocale == NULL) { return; }
        if(m_dLocales.count(sLocaleName) != 0) {
            m_pCurrentLocale = &m_dLocales.at(sLocaleName);
        }
    }

    bool CLocaleManager::MountLocale(const char* sName) { return false; }

    const char* CLocaleManager::GetString(const char* sKey) const {
        if(m_pCurrentLocale == NULL) { return m_sDefaultFallback; }
        const char* sResult = m_pCurrentLocale->GetString(sKey);
        if(sResult == NULL) {
            return m_sDefaultFallback;
        }

        return sResult;
    }
}
