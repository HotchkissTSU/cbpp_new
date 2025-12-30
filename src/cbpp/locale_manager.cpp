#include "cbpp/locale_manager.h"

#include "cbpp_api/Filesystem.h"

#include "jsoncpp/json/json.h"

// CLocale
namespace cbpp {
    const char* CLocale::GetFallbackString() const {
        return CBPP_LOCALE_FALLBACK;
    }

    const char* CLocale::GetString(const CString& sKey) const {
        const CString* sTest = m_dKeys.At(sKey);
        if(sTest != NULL) {
            return sTest->String();
        }

        return GetFallbackString();
    }

    bool CLocale::HasKey(const CString& sKey) const {
        return m_dKeys.HasKey(sKey);
    }
}

namespace cbpp {
    CLocaleManager* GetLocaleManager() {
        static CLocaleManager s_Manager;
        return &s_Manager;
    }

    void SetLocale(const char* sLocaleName) {
        CLocaleManager* pManager = GetLocaleManager();

        CLocale** pTest = pManager->m_dLocales.At(sLocaleName);
        if(pTest == NULL) { return; }

        pManager->m_pCurrentLocale = *pTest;
    }

    const CLocale* GetCurrentLocale() {
        CLocaleManager* pManager = GetLocaleManager();
        return pManager->m_pCurrentLocale;
    }

    bool MountLocale(const char* sFileName) {
        IFile* hFile = OpenFile(sFileName, "rb");

        if(!hFile->IsOpen()) { return false; }

        size_t iLength = hFile->Length();
        char* sJsonBuffer = Malloc<char>(iLength+1);
        sJsonBuffer[iLength] = '\0';
        hFile->ReadAll(sJsonBuffer);

        Json::Reader FileReader;
        Json::Value jRoot;

        if(!FileReader.parse(sJsonBuffer, sJsonBuffer + iLength, jRoot, false)) {
            return false;
        }

        printf("TEST = %s\n", jRoot["test"].asCString());

        CloseFile(hFile);
        return true;
    }
}
