#include "engine/locale_manager.h"

#include "cbpp/Filesystem.h"
#include "cbpp/CML.h"

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

        if(hFile == NULL) { return false; }

        size_t iLength = hFile->Length();
        char* sCmlBuffer = Malloc<char>(iLength+1);
        sCmlBuffer[iLength] = '\0';
        hFile->ReadAll(sCmlBuffer);
        CloseFile(hFile);

        cml::CParser Parser;
        if(!Parser.ParseString(sCmlBuffer)) {
            char sErrorBuff[256];
            Parser.GetErrorLog(sErrorBuff, sizeof(sErrorBuff));
            CbAssertf(true, "'%s' CML parsing error: '%s'", sFileName, sErrorBuff);
        }
        
        cml::IObject* pRoot = Parser.Root();

        cml::IObject* pPrettyName = pRoot->GetByName("name");
        CbAssertf(pPrettyName == NULL, "'%s' locale config has no 'name' field defined", sFileName);

        cml::IObject* pLocaleData = pRoot->GetByName("data");
        CbAssertf(pLocaleData == NULL, "'%s' locale config has no 'data' field defined", sFileName);

        

        return true;
    }
}
