#ifndef CBPP_API_FILESYSTEM_H
#define CBPP_API_FILESYSTEM_H

#include <stddef.h>
#include <stdint.h>

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
            IFile(const IFile&) = delete;
            IFile(IFile&&) = delete;

            IFile& operator=(IFile&&) = delete;
            IFile& operator=(const IFile&) = delete;

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
    void CloseFile(IFile* hFile);
}

#endif
