#include "cbpp/ref_string.h"

#include <string.h>

namespace cbpp {
    void CRefString::Set(const char* sSource, size_t iPos, size_t iLength) {
        m_pStart = (char*)(sSource) + iPos;
        m_pEnd = m_pStart + iLength;
    }

    CRefString::CRefString(const CString& sSource, size_t iPos, size_t iLength) {
        Set(sSource.String(), iPos, iLength);
    }

    CRefString::CRefString(const char* sSource, size_t iPos, size_t iLength) {
        Set(sSource, iPos, iLength);
    }

    size_t CRefString::Length() const {
        return (size_t)(m_pEnd - m_pStart);
    }

    char* CRefString::Start() const {
        return m_pStart;
    }

    char* CRefString::End() const {
        return m_pEnd;
    }

    size_t CRefString::Bufferize(char* pBuffer, size_t iBuffSize) const {
        size_t iCopySize = 0;

        if(Length() >= iBuffSize) {
            iCopySize = iBuffSize;
        } else {
            iCopySize = Length();
        }

        memcpy(pBuffer, m_pStart, iCopySize);
        pBuffer[iCopySize-1] = '\0';
        return iCopySize;
    }
}
