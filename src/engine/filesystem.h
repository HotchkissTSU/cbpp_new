#ifndef CBPP_FILES_API_H
#define CBPP_FILES_API_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "cbpp/String.h"
#include "cbpp/Filesystem.h"
#include "cbpp/Array.h"
#include "cbpp/Table.h"
#include "cbpp/Storage.h"

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
            bool IsVirtual() const override;
    };

    // File from the VFS
    class CVFile final : public IFile {
        friend IFile* OpenFile(const char*, const char*);
        friend void CloseFile(IFile*);

        void* m_pBegin = NULL;
        size_t m_iSize = 0;
        size_t m_iPointer = 0;
        bool m_bReadOnly = false;

        bool Open(const char* sFullPath, const char* sModes) override;
        void Close() override;

        void SetReadonly(bool);
        void SetBaseAddress(void*);

        public:
            size_t Write(size_t iCount, const void* pData) override;
            size_t Read(size_t iCount, void* pBuffer) override;

            size_t ReadAll(char* pBuff) const override;

            int GetChar() override;
            void PutChar(int iChar) override;
            bool IsEOF() const override;
            void Rewind() override;

            size_t Length() const override;
            bool IsOpen() const override;

            size_t GetPos() const override;
            size_t SetPos(size_t iOffset, EFileSeek iStart) override;

            bool IsVirtual() const override;
    };

    /*
        RAII can easily be turned on this way, but can not be disabled by any comprehensive means.
        So file handles are RAII-free by default.
    */
    class CFileWrapper {
        CVFile* m_pFile = NULL;

        public:
            CFileWrapper(CVFile* pFile);
            ~CFileWrapper();
    };

    class CVFileSystem {
        friend bool MountAssetPack(const char* sName, int32_t iPriority);

        CStorage<CFileWrapper> m_aFiles;
        CArray<CFileMap*> m_aMaps;

        public:
            SFileInfo GetFileInfo(const char* sPath) const;

            /*
                Open a virtual file.
                Note that if this file`s asset pack is mapped as read-only, file also will be so
            */
            IFile* OpenFile(const char* sPath, const char* sModes);

            void CloseFile(IFile* pFile);
    };

    // Get a virtual file system handler
    CVFileSystem* GetVFS();
}

#endif
