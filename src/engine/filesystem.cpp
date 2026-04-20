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
#endif

#ifdef CBPP_WINDOWS
    #include <direct.h>
    #include <windef.h>
    #define cbpp_getcwd(_buff, _size) _getcwd(_buff, _size)
#endif

#ifndef cbpp_getcwd
    #error "Unsupported platform"
#endif

// Generic interface
namespace cbpp {
    const char* GetCWD(char* pBuffer, size_t iBufferSize) {
        return cbpp_getcwd(pBuffer, iBufferSize);
    }

    inline bool IsSlash(char iChar) {
        return (iChar == '/' || iChar == '\\');
    }

    // Works with UTF-8 input on Linux (god bless POSIX), untested on other systems

    const char* ValidatePath(const char* sPath, char* sBuffer, size_t iBufferSize) {
        const size_t iBufLen = (iBufferSize == 0) ? PATH_MAX : iBufferSize;

        if(sBuffer == NULL) { sBuffer = Malloc<char>(iBufLen); }

        static const char* s_sAssetPrefix = "assets/";

        size_t iPrefixLength = Min(sizeof(s_sAssetPrefix) - 1, iBufLen);
        memcpy(sBuffer, s_sAssetPrefix, iPrefixLength);

        sBuffer += iPrefixLength;

        char* pCurrent = (char*)sPath;
        size_t i = 0;

        while(IsSlash(*pCurrent)) { pCurrent++; }     // skip starting slashes

        while(*pCurrent != '\0') {
            if(*pCurrent == '.' && *(pCurrent+1) == '.') {
                // purge any '..' masks to entrap path in the "assets/" subdirectory
                while((*pCurrent == '.' || IsSlash(*pCurrent)) && *pCurrent != '\0') { pCurrent++; }
            }

            // purge '~' home aliases for Linux PCs
            CBPP_ON_LINUX (
                if(*pCurrent == '~') {
                    while((*pCurrent == '~' || IsSlash(*pCurrent)) && *pCurrent != '\0') { pCurrent++; }
                }
            )
            
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
            if(*pCurrent == '\\') {     // me and the boys only adore forward slashes
                *pCurrent = '/';
            }
            pCurrent++;
        }

        return sBuffer - iPrefixLength;
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
        char sBuffer[PATH_MAX];
        ValidatePath(sPath, sBuffer, sizeof(sBuffer));

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
        return fread(pData, 1, iCount, m_hFile);
    }

    size_t CFile::ReadAll(char* pBuffer) const {        
        size_t iLength = Length();
        return fread(pBuffer, 1, iLength, m_hFile);
    }

    int CFile::GetChar() const {
        return fgetc(m_hFile);
    }
}
