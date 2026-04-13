#ifndef CBPP_API_FILESYSTEM_H
#define CBPP_API_FILESYSTEM_H

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>

#include "cbpp/String.h"
#include "cbpp/Constants.h"

namespace cbpp {
    enum class EAssetPath : uint32_t {
        Invalid,

        Generic,
        Sprite,
        Locale,
        Audio,
        Map,
        Model,

        AMOUNT
    };

    const char* GetCWD(char* pBuffer, size_t iBufferSize);

    /*
        Entrap a path inside the "assets/" directory thus ensuring players` safety.
        All asset paths which can be sourced from runtime must be passed though this call.

        If NULL is passed as target buffer, then a new buffer is allocated.
        If iBufferSize is not zero, allocated buffer is this size, else PATH_MAX.
    */
    const char* ValidatePath(const char* sPath, char* sBuffer, size_t iBufferSize);

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

            // Write some data into the file. Returns actual amount of bytes written
            virtual size_t Write(size_t iCount, const void* pData) = 0;

            // Read some data from the file. Returns actual amount of bytes read
            virtual size_t Read(size_t iCount, void* pBuffer) = 0;
            virtual size_t ReadAll(char* pBuff) const = 0;

            virtual size_t Length() const = 0;
            virtual bool IsOpen() const = 0;
            virtual FILE* Handle() = 0;
    };

    IFile* OpenFile(const char* sPath, const char* sModes);
    IFile* OpenAsset(const char* sPath, const char* sModes);
    void CloseFile(IFile* hFile);

    enum class EFileClass : uint32_t {
        File,
        Directory,
        CharDevice
    };

    struct FileInfo {
        bool bExists;
        size_t iSize;
        EFileClass iClass;
        time_t iLastAccess, iLastModified;
    };

    FileInfo GetFileInfo(const char* sPath);

    class CDirIterator {
        CString m_sPath;

        public:
            CDirIterator() = default;
            CDirIterator(const char* sPath);

            void SetPath(const char* sPath);

            void Advance();
            void operator++();

            const char* Current();
            explicit operator cstring_t();
    };
}

#endif
