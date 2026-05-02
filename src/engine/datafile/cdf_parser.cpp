/*#include "cbpp/CDF.h"

#include <ctype.h>

#include "cbpp/Array.h"

#define CDF_HEAD (*((uint32_t*)("CDF")))

namespace cbpp::cdf {
    EBinaryError CBinaryParser::ReadHeader() {
        Header Head;

        size_t iRead = m_pFile->Read(sizeof(Head), &Head);

        if(iRead != sizeof(Head)) { return EBinaryError::UnexpectedEOF; }

        if( Head.iHead != CDF_HEAD ) {
            return EBinaryError::HeaderMismatch;
        }

        if( Head.Version.iMajor != CBPP_CDF_VERSION_MAJOR ) {
            return EBinaryError::VersionMismatch;
        }

        m_iUserType = Head.iUserType;

        return EBinaryError::Ok;
    }

    EBinaryError CBinaryParser::ReadNametable() {
        uint32_t iTableLength;
        size_t iRead = m_pFile->Read(sizeof(iTableLength), &iTableLength);

        if(iRead != sizeof(iTableLength)) { return EBinaryError::UnexpectedEOF; }

        int iCurrent;
        
        cbpp::CArray<char> sCurrentName;

        for(size_t i = 0; i < iTableLength; i++) {
            while(true) {
                iCurrent = m_pFile->GetChar();

                if(iCurrent == EOF) {
                    return EBinaryError::UnexpectedEOF;
                }

                if( !(isalnum(iCurrent) || iCurrent == '_' || iCurrent == '\0') ) {
                    return EBinaryError::InvalidName;
                }

                sCurrentName.PushBack(iCurrent);

                if(iCurrent == '\0') { break; }
            }

            m_aNameTable.AddOrRef(sCurrentName.Data());
            sCurrentName.Clear();
        }

        return EBinaryError::Ok;
    }

    EBinaryError CBinaryParser::Parse(const char* sPath) {
        m_pFile = cbpp::OpenFile(sPath, "rb");

        if(m_pFile == NULL) {
            return EBinaryError::NoFile;
        }

        EBinaryError iRet = this->ReadHeader();
        if(iRet != EBinaryError::Ok) { return iRet; }
    }
}
*/
