#include "engine/filesystem.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "cbpp/Bit.h"

#include "cbpp/Memory.h"
#include "cbpp/Math.h"
#include "cbpp/String.h"
#include "cbpp/Constants.h"

#ifdef CBPP_LINUX
    #include <unistd.h>
    #include <limits.h>
    #define cbpp_getcwd(_buff, _size) getcwd(_buff, _size)
#else
    #include <direct.h>
    #include <windef.h>
    #define cbpp_getcwd(_buff, _size) _getcwd(_buff, _size)
#endif

// Generic interface
namespace cbpp {
    const char* GetCWD(char* pBuffer, size_t iBufferSize) {
        return cbpp_getcwd(pBuffer, iBufferSize);
    }

    inline bool IsSlash(char iChar) {
        return (iChar == '/' || iChar == '\\');
    }

    const char* ValidatePath(const char* sPath, char* sBuffer, size_t iBufferSize) {
        const size_t iBufLen = (iBufferSize == 0) ? PATH_MAX : iBufferSize;

        if(sBuffer == NULL) {
            sBuffer = Malloc<char>(iBufLen);
        }

        static const char* s_sAssetPrefix = "assets/";

        size_t iPrefixLength = Min(sizeof(s_sAssetPrefix) - 1, iBufLen);
        memcpy(sBuffer, s_sAssetPrefix, iPrefixLength);

        sBuffer += iPrefixLength;   // offset the buffer to skip the prefix

        char* pCurrent = (char*)sPath;
        size_t i = 0;

        while(IsSlash(*pCurrent)) { pCurrent++; }     // skip starting slashes

        while(*pCurrent != '\0') {
            if(*pCurrent == '.' && *(pCurrent+1) == '.') {
                // purge any '..' masks to entrap path in the "assets/"" subdirectory
                while((*pCurrent == '.' || IsSlash(*pCurrent)) && *pCurrent != '\0') { pCurrent++; }
            }

            if(*pCurrent == '~') {
                // purge '~' home aliases for Linux PCs
                while((*pCurrent == '~' || IsSlash(*pCurrent)) && *pCurrent != '\0') { pCurrent++; }
            }
            
            sBuffer[i] = *pCurrent;

            i++;
            pCurrent++;

            if((iPrefixLength + i) == iBufLen-1 || *pCurrent == '\0') {
                sBuffer[i] = '\0';
                break;
            }
        }
        
        pCurrent = sBuffer;
        while(*pCurrent != '\0') {      // purge any w*ndows '\' path separators
            if(*pCurrent == '\\') {
                *pCurrent = '/';
            }
            pCurrent++;
        }

        return sBuffer - iPrefixLength; // offset our pointer backwards
    }

    IFile* OpenFile(const char* sPath, const char* sModes) {
        CFile* pFile = New<CFile>();
        bool bOpen = pFile->Open(sPath, sModes);

        if(bOpen) {
            return (IFile*)(pFile);
        }else{
            WriteLogf(ELogLevel::Warning, "Failed to open file '%s' : %s", sPath, strerror(errno));

            Delete(pFile);
            return NULL;
        }
    }

    IFile* OpenAsset(const char* sPath, const char* sModes) {
        char sBuffer[512];
        snprintf(sBuffer, sizeof(sBuffer), "assets/%s", sPath);

        return OpenFile(sBuffer, sModes);
    }

    void CloseFile(IFile* hFile) {
        if(hFile != NULL) {
            hFile->Close();
        }
    }
}

// CFile
namespace cbpp {
    FILE* CFile::Handle() { return m_hFile; }

    size_t CFile::Length() const {
        if(!IsOpen()) {
            return 0;
        }

        long iOldPos = ftell(m_hFile);

        fseek(m_hFile, 0, SEEK_END);
        size_t iLength = (size_t)(ftell(m_hFile));
        fseek(m_hFile, iOldPos, SEEK_SET);

        return iLength;
    }

    bool CFile::IsOpen() const {
        return m_hFile != NULL;
    }

    bool CFile::Open(const char* sPath, const char* sModes) {
        m_hFile = fopen(sPath, sModes);
        return m_hFile != NULL;
    }

    void CFile::Close() {
        if(m_hFile != NULL) {
            fclose(m_hFile);
            m_hFile = NULL;
        }
    }
    
    size_t CFile::Write(size_t iCount, const void* pData) {
        return fwrite(pData, 1, iCount, m_hFile);
    }

    size_t CFile::Read(size_t iCount, void* pData) {
        return fwrite(pData, 1, iCount, m_hFile);
    }

    size_t CFile::ReadAll(char* pBuffer) const {        
        size_t iLength = Length();
        return fread(pBuffer, 1, iLength, m_hFile);
    }
}

namespace cbpp {
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
