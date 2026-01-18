#include "cbpp/String.h"

#include <string.h>

namespace cbpp {
    void CSubString::Set(const char* sSource, size_t iPos, size_t iLength) {
        m_pStart = (char*)(sSource) + iPos;
        m_pEnd = m_pStart + iLength;
    }

    CSubString::CSubString(const CString& sSource, size_t iPos, size_t iLength) {
        Set(sSource.String(), iPos, iLength);
    }

    CSubString::CSubString(const char* sSource, size_t iPos, size_t iLength) {
        Set(sSource, iPos, iLength);
    }

    size_t CSubString::Length() const {
        return (size_t)(m_pEnd - m_pStart);
    }

    char* CSubString::Start() const {
        return m_pStart;
    }

    char* CSubString::End() const {
        return m_pEnd;
    }

    size_t CSubString::Bufferize(char* pBuffer, size_t iBuffSize) const {
        size_t iCopySize = 0;
        const size_t iLength = Length();

        if(iLength >= iBuffSize) {
            iCopySize = iBuffSize;
        } else {
            iCopySize = iLength+1;
        }

        memcpy(pBuffer, m_pStart, iCopySize);
        pBuffer[iCopySize-1] = '\0';
        return iCopySize;
    }

    CString CSubString::Copy() const {
        const size_t iLength = Length();
        CString sOut(iLength);
        memcpy(sOut.Pointer(), m_pStart, iLength);

        return sOut;
    }

    bool CSubString::operator==(const CSubString& Other) const {
        return SubStringCmp(*this, Other) == 0;
    }

    bool CSubString::operator<(const CSubString& Other) const {
        return SubStringCmp(*this, Other) < 0;
    }

    bool CSubString::operator>(const CSubString& Other) const {
        return SubStringCmp(*this, Other) > 0;
    }

    int32_t SubStringCmp(const CSubString& sA, const CSubString& sB) {
        size_t iLenA = sA.Length(), iLenB = sB.Length();

        if(iLenA < iLenB) {
            return -1;
        } else if(iLenB > iLenA) {
            return 1;
        }

        char *pA = sA.Start(), *pB = sB.Start();
        for(size_t i = 0; i < iLenA; i++) {
            if((*pA) < (*pB)) {
                return -1;
            } else if((*pA) > (*pB)) {
                return 1;
            }

            pA++; pB++;
        }

        return 0;
    }
}
