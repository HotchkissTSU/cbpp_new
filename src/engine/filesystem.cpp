#include "engine/filesystem.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "cbpp/Bit.h"

#include "cbpp/Memory.h"
#include "cbpp/Math.h"
#include "cbpp/String.h"
#include "cbpp/Constants.h"
#include "cbpp/Error.h"

#ifdef CBPP_LINUX
    #include <unistd.h>
    #include <limits.h>
    #define cbpp_getcwd(_buff, _size) getcwd(_buff, _size)

    #include <fcntl.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

#ifdef CBPP_WINDOWS
    #include <direct.h>
    #include <windef.h>
    #include <windows.h>
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
        SFileInfo fileData = GetFileInfo(sPath);

        if(!fileData.exists) { return NULL; }

        if( fileData.fileClass == EFSClass::File ) { // First, we try opening a real file
            CFile* pFile = New<CFile>();
            if( !pFile->Open(sPath, sModes) ) {
                Delete(pFile);
                return NULL;
            }

            return (IFile*)pFile;

        } else if( fileData.fileClass == EFSClass::VFile) { // no real file found, open the VFS`s one
            CVFile* pFile = New<CVFile>();
            if( !pFile->Open(sPath, sModes) ) {
                Delete(pFile);
                return NULL;
            }

            return (IFile*)pFile;
        }

        return NULL;
    }

    /*IFile* OpenFile(const char* sPath, const char* sModes) {
        CFile* pFile = New<CFile>();
        bool bOpen = pFile->Open(sPath, sModes);

        if(bOpen) {
            return (IFile*)(pFile);
        }else{
            WriteLogf(ELogLevel::Warning, "Failed to open file '%s' : %s", sPath, strerror(errno));

            Delete(pFile);
            return NULL;
        }
    }*/

    void CloseFile(IFile* hFile) {
        if(hFile != NULL) {
            hFile->Close();
        }
    }

    SFileInfo GetFileInfo(const char* sPath) {
        return GetFileInfoP(sPath); // PLACEHOLDER
    }

    SFileInfo GetFileInfoP(const char* sPath) {
        SFileInfo out = {};
        out.exists = false;

        #ifdef CBPP_WINDOWS
            CBPP_NO_WINDOWS_ASSERT
        #endif

        #ifdef CBPP_LINUX
            int iFD = open(sPath, O_RDONLY);
            if(iFD < 0) {
                return out;
            }

            struct stat fileInfo;
            if( fstat(iFD, &fileInfo) < 0 ) {
                return out;
            }

            out.exists = true;

            out.byteSize = fileInfo.st_size;
            out.fileClass = EFSClass::File;

            if(iFD) { close(iFD); }
        #endif

        return out;
    }
}

// CFile
namespace cbpp {
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

    void CFile::Rewind() {
        rewind(m_hFile);
    }

    size_t CFile::GetPos() const {
        return ftell(m_hFile);
    }

    size_t CFile::SetPos(size_t iOffset, EFileSeek iStart) {
        return fseek(m_hFile, iOffset, (int)iStart);
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

    int CFile::GetChar() {
        return fgetc(m_hFile);
    }

    void CFile::PutChar(int iChar) {
        putc(iChar, m_hFile);
    }

    bool CFile::IsEOF() const {
        return feof(m_hFile) != 0;
    }
}

// CVFile

namespace cbpp {
    bool CVFile::Open(const char* sFullPath, const char* sModes) { return false; }
    void CVFile::Close() {}

    size_t CVFile::Write(size_t iCount, const void* pData) {
        if( m_bReadOnly == true ) { return 0; }
        if( this->IsEOF() ) { return 0; }

        size_t iWrite = (m_iPointer + iCount < m_iSize) ? iCount : (m_iSize - m_iPointer);

        memcpy( (char*)(m_pBegin) + m_iPointer, pData, iWrite );
        m_iPointer += iWrite;

        return iWrite;
    }
    
    size_t CVFile::Read(size_t iCount, void* pBuffer) {
        if(this->IsEOF()) { return 0; }

        size_t iRead = (m_iPointer + iCount < m_iSize) ? iCount : (m_iSize - m_iPointer);

        memcpy( pBuffer, (char*)(m_pBegin) + m_iPointer, iCount );
        m_iPointer += iRead;

        return iRead;
    }

    size_t CVFile::ReadAll(char* pBuffer) const {
        memcpy(pBuffer, m_pBegin, m_iSize);
        return m_iSize;
    }
    
    int CVFile::GetChar() {
        if( this->IsEOF() ) { return -1; }
        return *((char*)m_pBegin + (m_iPointer++));
    }

    void CVFile::PutChar(int iChar) {
        if(this->IsEOF()) { return; }
        *((char*)m_pBegin + (m_iPointer++)) = (char)(iChar);
    }

    bool CVFile::IsEOF() const {
        if( m_pBegin == NULL ) { return true; }
        return m_iPointer > m_iSize;
    }

    void CVFile::Rewind() {
        m_iPointer = 0;
    }

    size_t CVFile::Length() const {
        return m_iSize;
    }

    bool CVFile::IsOpen() const {
        return m_pBegin != NULL;
    }

    size_t CVFile::GetPos() const {
        return m_iPointer;
    }

    size_t CVFile::SetPos(size_t iOffset, EFileSeek iStart) {
        switch( iStart ) {
            case EFileSeek::Start:      m_iPointer = iOffset; break;
            case EFileSeek::Current:    m_iPointer += iOffset; break;
            case EFileSeek::End:        m_iPointer = m_iSize - iOffset; break;
        }

        return m_iPointer;
    }
}

// CVFileSystem

namespace cbpp {
    bool MountAssetPack(const char* sName, int32_t iPriority) {
        

        return true;
    }
}

// CFileMap

namespace cbpp {        
    CFileMap* MapFile(const char* sPath, bool bAllowWriting) {
        #ifdef CBPP_WINDOWS
            CBPP_NO_WINDOWS_ASSERT
        #endif

        #ifdef CBPP_LINUX
            int iFD = open(sPath, bAllowWriting ? O_RDWR : O_RDONLY);

            if(iFD < 0) {
                WriteLogf(ELogLevel::Error, "Can`t open '%s' for mapping (%s)", sPath, strerror(errno));
                return NULL;
            }
            
            struct stat fileInfo;
            if( fstat(iFD, &fileInfo) < 0 ) {
                WriteLogf(ELogLevel::Error, "fstat() failed: %s", strerror(errno));
                close(iFD);
                return NULL;
            }

            void* pData = mmap(NULL, fileInfo.st_size, bAllowWriting ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED, iFD, 0);

            CFileMap* pMap = New<CFileMap>();
            pMap->m_iFD = iFD;
            pMap->m_pData = pData;
            pMap->m_iLength = fileInfo.st_size;

            return pMap;
        #endif
    }

    void UnmapFile(CFileMap* pMap) {
        Delete(pMap);
    }

    void CFileMap::Sync(bool bAsync) {
        msync(m_pData, m_iLength, bAsync ? MS_ASYNC : MS_SYNC);
    }

    CFileMap::operator uint8_t* () {
        return (uint8_t*)m_pData;
    }

    uint8_t* CFileMap::Data() {
        return (uint8_t*)m_pData;
    }

    size_t CFileMap::Length() const {
        return m_iLength;
    }

    CFileMap::~CFileMap() {
        #ifdef CBPP_LINUX
            if(m_iFD > 0) {
                msync(m_pData, m_iLength, MS_SYNC);
                close(m_iFD);
                munmap(m_pData, m_iLength);
            }
        #endif
    }
}
