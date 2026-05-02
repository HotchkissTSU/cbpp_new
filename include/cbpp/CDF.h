#ifndef CBPP_CDF_API_H
#define CBPP_CDF_API_H

#include "cbpp/Filesystem.h"

#include "engine/datafile/object_model.h"

#define CBPP_CDF_VERSION_MAJOR 1
#define CBPP_CDF_VERSION_MINOR 0
#define CBPP_CDF_VERSION_PATCH 0

#define CBPP_CDF_BLOCK_SIZE 512

namespace cbpp::cdf {
    enum class EBinaryError : uint32_t {
        Ok,                                 // Success
        NoFile,                             // File not found
        HeaderMismatch,                     // File is not a proper CDF
        UnexpectedEOF,                      // EOF jumpscare where it shouldn`t be
        VersionMismatch,                    // CDF major version is mismatched
        InvalidName,                        // Nametable entry has an invalid name in it
    };

    struct BinaryHeader {
        union {
            char aHead[4];
            uint32_t iHead;
        };

        struct {
            uint16_t iMajor, iMinor, iPatch;
        } Version;

        uint16_t iUserType;
    };
    
    struct BinaryBlockHeader {
        uint8_t iNameIndex;
        uint8_t iTypeIndex;

        uint32_t iDataLength;
    };

    struct BinaryBlock {
        union {
            BinaryBlockHeader Head;

            char aBytes[CBPP_CDF_BLOCK_SIZE];
        };
    };

    class CBinaryParser {
        CObject m_pRootObject = cdf::NIL;
        cbpp::IFile* m_pFile = NULL;

        cbpp::CStringPool m_aNameTable;

        uint16_t m_iUserType;

        EBinaryError ReadHeader();
        EBinaryError ReadNametable();

        public:
            CBinaryParser() = default;

            EBinaryError Parse(const char* sPath);

            void Clear();

            CObject Root() const;
            CObject operator[](const char* sPath);

            size_t FormatError(EBinaryError iCode, char* pBuffer, size_t iSize) const;
    };
}

#endif
