#include "cbpp/filesystem.h"

#include <string.h>
#include <stdio.h>

#include "cbpp/cbdef.h"
#include "cbpp/bit.h"
#include "cbpp_api/Math.h"

namespace cbpp {
    CAssetPathGroup::CAssetPathGroup(const char* sFallback) : m_sFallbackPath(sFallback) {}

    void CAssetPathGroup::PushPath(const char* sPath) {
        CImmutableString sTmp(sPath);
        m_aPaths.PushBack(sTmp);
    }

    size_t CAssetPathGroup::Length() { return m_aPaths.Length(); }

    const CImmutableString& CAssetPathGroup::operator[](size_t iIndex) {
        return (const CImmutableString&)(m_aPaths[iIndex]);
    }

    CAssetPathsManager::CAssetPathsManager() {}

    CAssetPathsManager* GetPathsManager() {
        static CAssetPathsManager s_hPathsManager;
        return &s_hPathsManager;
    }

    bool CFile::Open(const char* sPath, const char* sModes) {
        m_hFile = fopen(sPath, sModes);
        return m_hFile != NULL;
    }

    size_t CFile::Read(size_t iCount, void* pBuffer) {
        return fread(pBuffer, 1, iCount, m_hFile);
    }

    size_t CFile::Write(size_t iCount, const void* pData) {
        return fwrite(pData, 1, iCount, m_hFile);
    }

    bool CFile::IsOpen() { return (m_hFile != NULL); }

    void CFile::Close() { if(m_hFile != NULL) { fclose(m_hFile); } }

    void CAssetPathsManager::RegisterPath(EAssetPath iGroup, const char* sPrefix) {
        m_aGroups[(int)iGroup].PushPath(sPrefix);
    }

    const char* CAssetPathsManager::FindPath(EAssetPath iGroup, const char* sShortPath) {
        static char s_sPathBuffer[FILENAME_MAX];

        s_sPathBuffer[0] = '\0';
        for( size_t i = 0; i < (size_t)EAssetPath::AMOUNT; i++ ) {
            for( size_t j = 0; j < m_aGroups[i].Length(); j++ ) {
                const char* sCurrentPrefix = m_aGroups[i][j];

                snprintf(s_sPathBuffer, FILENAME_MAX, "%s/%s", sCurrentPrefix, sShortPath);

                printf("%s\n", s_sPathBuffer);
            }
        }

        return NULL;
    }

    void RegisterSearchPath(EAssetPath iGroup, const char* sPath) {
        CAssetPathsManager* pManager = GetPathsManager();
        pManager->RegisterPath(iGroup, sPath);
    }

    void ParsePath(const char* sPath) {
        EFileSystem iTargetFS;
        char sFSNameBuff[8];
        const char* pColon = strchr(sPath, ':');
        char* pCurrent;

        if(pColon == NULL) {    // No FS is specified, assume default
            iTargetFS = EFileSystem::Physical;
        }else {
            const ptrdiff_t iFSNameLen = pColon - sPath;
            memcpy(sFSNameBuff, sPath, Clamp(iFSNameLen, (ptrdiff_t)0, (ptrdiff_t)8));
            sFSNameBuff[iFSNameLen] = '\0';

            if( strcmp(sFSNameBuff, "CPK") == 0 ) {
                iTargetFS = EFileSystem::Virtual;
            } else if( strcmp(sFSNameBuff, "OS") == 0 ) {
                iTargetFS == EFileSystem::Physical;
            }
            printf("FS: %s\n", sFSNameBuff);
        }

        pCurrent = (char*)strchr(sPath, '/') + 1;
        
        while(pCurrent != NULL) {            
            const char* pBlockEnd = strchr(pCurrent+1, '/');
            if(pBlockEnd == NULL) {
                printf("%s\n", pCurrent);
                break;
            }

            for(char* pC = pCurrent; pC != pBlockEnd && *pC != '\0'; pC++) {
                putc(*pC, stdout);
            }
            putc('\n', stdout);

            pCurrent = (char*)pBlockEnd + 1;
        }
    }
}
