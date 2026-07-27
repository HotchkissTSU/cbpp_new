#ifndef CBPP_API_FILESYSTEM_H
#define CBPP_API_FILESYSTEM_H

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>

#include "cbpp/Constants.h"

namespace cbpp {
    const char* GetCWD(char* pBuffer, size_t iBufferSize);

    /*
        Entrap a path inside the "assets/" directory thus ensuring players` safety.
        All asset paths which can be sourced from runtime must be passed though this call.

        If NULL is passed as target buffer, then a new buffer is allocated.
        If iBufferSize is not zero, allocated buffer is this size, else PATH_MAX.
    */
    const char* ValidatePath(const char* sPath, char* sBuffer, size_t iBufferSize);

    enum class EFileSeek : uint8_t {
        Start = SEEK_SET,
        End = SEEK_END,
        Current = SEEK_CUR
    };

    /*
        General file I/O interface
    */
    class IFile {
        friend IFile* OpenFile(const char*, const char*);
        friend void CloseFile(IFile*);

        protected:
            virtual bool Open(const char* sFullPath, const char* sModes) = 0;
            virtual void Close() = 0;

        public:
            IFile() = default;
            
            CBPP_PROTECTED_CLASS(IFile)

            virtual size_t Write(size_t iCount, const void* pData) = 0;
            virtual size_t Read(size_t iCount, void* pBuffer) = 0;
            virtual size_t ReadAll(char* pBuff) const = 0;

            virtual int GetChar() = 0;
            virtual void PutChar(int iChar) = 0;
            virtual bool IsEOF() const = 0;
            virtual void Rewind() = 0;

            virtual size_t Length() const = 0;
            virtual bool IsOpen() const = 0;

            virtual bool IsVirtual() const = 0;

            virtual size_t GetPos() const = 0;
            virtual size_t SetPos(size_t, EFileSeek) = 0;
    };

    IFile* OpenFile(const char* sPath, const char* sModes);
    void CloseFile(IFile* hFile);

    // Filesystem entity class
    enum class EFSClass : uint32_t {
        File,                           // Real file from disk
        VFile,                          // Virtual file
        Directory,                      // Real directory
        VDirectory                      // Virtual directory
    };

    struct SFileInfo {
        bool exists;
        size_t byteSize;
        EFSClass fileClass;
        uint64_t lastAccess, lastModif;
    };

    // Get info on a file, physical or virtual
    SFileInfo GetFileInfo(const char* sPath);

    // Get info on a physical file explicitly
    SFileInfo GetFileInfoP(const char* sPath);

    class CFileMap {
        friend CFileMap* MapFile(const char*, bool);
        friend void UnmapFile(CFileMap*);

        void* m_pData = NULL;
        size_t m_iLength = 0;          // user length

        #ifdef CBPP_LINUX
            int m_iFD;
        #endif

        #ifdef CBPP_WINDOWS
            void* m_hFile, m_hMapping;      // void* is used instead of HANDLE to avoid polluting this API header with <windows.h> garbage
        #endif

        void Sync(bool bAsync = false);

        public:
            CBPP_PROTECTED_CLASS(CFileMap)

            CFileMap() = default;
            ~CFileMap();

            uint8_t* Data();
            size_t Length() const;

            operator uint8_t*();
    };

    // Map a file to the virtual address space
    CFileMap* MapFile(const char* sPath, bool bAllowWriting = true);

    // Unmap a file
    void UnmapFile(CFileMap*);

    bool MountAssetPack(const char* sName);
}

#endif
