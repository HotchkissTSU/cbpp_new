#ifndef CBPP_FILES_API_H
#define CBPP_FILES_API_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "cbpp/cbstring.h"
#include "cbpp_api/Filesystem.h"
#include "cbpp_api/Array.h"

namespace cbpp {
    // File from OS disk
    class CFile final : public IFile {
        friend IFile* OpenFile(const char*, const char*);
        friend void CloseFile(IFile*);

        FILE* m_hFile = NULL;

        bool Open(const char* sFullPath, const char* sModes) override;
        void Close() override;

        public:
            size_t Write(size_t iCount, const void* pData) override;
            size_t Read(size_t iCount, void* pData) override;

            bool IsOpen() override;
    };

    // File from the virtual FS
    class CVirtualFile final : public IFile {
        friend IFile* OpenFile(const char*, const char*);
        friend void CloseFile(IFile*);

        bool Open(const char* sFullPath, const char* sModes) override;
        void Close() override;

        public:
            size_t Write(size_t iCount, const void* pData) override;
            size_t Read(size_t iCount, void* pData) override;

            bool IsOpen() override;
    };

    enum class EFileSystem {
        Physical,               // OS filesystem
        Virtual                 // Engine virtual FS
    };

    void ParsePath(const char* sPath);
}

#endif
