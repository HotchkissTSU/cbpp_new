#ifndef CBPP_FILES_API_H
#define CBPP_FILES_API_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "cbpp/immutable_string.h"
#include "cbpp_api/Filesystem.h"
#include "cbpp_api/Array.h"

namespace cbpp {
    class CAssetPathGroup {
        CImmutableString m_sFallbackPath;
        CArray<CImmutableString> m_aPaths;

        public:
            CAssetPathGroup() = default;
            CAssetPathGroup(const char* sFallbackPath);

            void PushPath(const char* sPath);

            size_t Length();
            const CImmutableString& operator[](size_t iIndex);
    };

    class CAssetPathsManager {
        friend CAssetPathsManager* GetPathsManager();

        CAssetPathGroup m_aGroups[(int)EAssetPath::AMOUNT];

        public:
            CAssetPathsManager();
            const char* FindPath(EAssetPath iGroup, const char* sShortPath);
            void RegisterPath(EAssetPath iGroup, const char* sPrefix);
    };

    CAssetPathsManager* GetPathsManager();

    // File from OS disk
    class CFile : public IFile {
        FILE* m_hFile = NULL;

        bool Open(const char* sFullPath, const char* sModes);
        void Close();

        public:
            size_t Write(size_t iCount, const void* pData);
            size_t Read(size_t iCount, void* pData);

            bool IsOpen();
    };

    // File from the virtual FS
    class CVirtualFile : public IFile {
        
    };

    enum class EFileSystem {
        Physical,               // OS filesystem
        Virtual                 // Engine virtual FS
    };

    void ParsePath(const char* sPath);
}

#endif
