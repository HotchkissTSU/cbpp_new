#ifndef CBPP_FILES_API_H
#define CBPP_FILES_API_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "cbpp/String.h"
#include "cbpp/Filesystem.h"
#include "cbpp/Array.h"
#include "cbpp/Table.h"

namespace cbpp {
    // File from OS disk
    class CFile final : public IFile {
        friend IFile* OpenFile(const char*, const char*);
        friend void CloseFile(IFile*);

        FILE* m_hFile = NULL;

        bool Open(const char* sFullPath, const char* sModes) override;
        void Close() override;

        public:
            size_t Length() const override;

            size_t Write(size_t iCount, const void* pData) override;
            size_t Read(size_t iCount, void* pData) override;
            size_t ReadAll(char* pBuff) const override;

            int GetChar() override;
            void PutChar(int iChar) override;
            bool IsEOF() const override;

            void Rewind() override;
            size_t GetPos() const override;
            size_t SetPos(size_t iOffset, EFileSeek iStart) override;

            bool IsOpen() const override;
    };

    // File from the VFS
    class CVFile : public IFile {
        friend IFile* OpenFile(const char*, const char*);
        friend void CloseFile(IFile*);

        void* m_pBegin = NULL;
        size_t m_iSize = 0;
        size_t m_iPointer = 0;
        bool m_bReadOnly = false;

        bool Open(const char* sFullPath, const char* sModes) override;
        void Close() override;

        void SetReadonly(bool);

        public:
            size_t Write(size_t iCount, const void* pData) override;

            // Attempt to read N bytes from the file. Returns the amount read successfully
            size_t Read(size_t iCount, void* pBuffer) override;

            // Read an entire file to the buffer
            size_t ReadAll(char* pBuff) const override;

            int GetChar() override;
            void PutChar(int iChar) override;
            bool IsEOF() const override;
            void Rewind() override;

            size_t Length() const override;
            bool IsOpen() const override;

            size_t GetPos() const override;
            size_t SetPos(size_t iOffset, EFileSeek iStart) override;
    };

    class CVFileSystem {
        friend bool MountAssetPack(const char* sName, int32_t iPriority);

        public:
            SFileInfo GetFileInfo(const char* sPath) const;
    };

    bool MountAssetPack(const char* sName, int32_t iPriority = -1);
}

#endif
