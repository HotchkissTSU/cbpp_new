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
        friend IFile* OpenFile(EAssetPath iPathGroup, const char* sShortPath);
        friend void CloseFile(IFile* hFile);

        virtual bool Open(const char* sFullPath, const char* sModes) = 0;
        virtual void Close() = 0;

        public:
            // Write some data into the file. Returns actual amount of bytes written
            virtual size_t Write(size_t iCount, const void* pData) = 0;

            // Read some data from the file. Returns actual amount of bytes read
            virtual size_t Read(size_t iCount, void* pBuffer) = 0;

            virtual bool IsOpen() = 0;
    };


    IFile* OpenFile(EAssetPath iPathGroup, const char* sShortPath, const char* sModes);

    // Open a file without path resolving
    IFile* OpenFileDirect(const char* sPath, const char* sModes);

    void CloseFile(IFile* hFile);

    void RegisterSearchPath(EAssetPath iGroup, const char* sPath);
}

#endif
