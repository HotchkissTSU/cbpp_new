#include "cbpp/String.h"

#include <string.h>

#include "cbpp/Memory.h"

namespace cbpp {
    const char* CStringPool::Data() const {
        return (const char*)m_pData;
    }

    size_t CStringPool::Length() const {
        return m_iCount;
    }

    size_t CStringPool::Size() const {
        return m_iBytes;
    }

    size_t CStringPool::PushString(const char* sData) {
        const size_t iLen = strlen(sData);
        m_pData = Realloc(m_pData, m_iBytes + iLen + 1);
        m_pData[m_iBytes + iLen] = '\0';

        memcpy(m_pData + m_iBytes, sData, iLen);

        m_iBytes += iLen;
        m_iCount++;

        return m_iBytes - iLen;
    }

    const char* CStringPool::operator[](size_t iIndex) const {
        if(m_pData == NULL) { return "(null)"; }
        return (const char*)(m_pData + iIndex);
    }

    const char* CStringPool::At(size_t iIndex) const {
        if(m_pData == NULL) { return "(null)"; }
        if(iIndex >= m_iBytes) { return "(null)"; }

        return (const char*)(m_pData + iIndex);
    }

    size_t CStringPool::Find(const char* sData) const {
        if(m_pData == NULL) { return -1; }

        char* pCurrent = m_pData;
        size_t i = 0;

        while(i < m_iCount) {
            char iCurrent = *pCurrent;

            if(i == 0 || iCurrent == '\0') {
                char* pString = pCurrent + ((i == 0) ? 0 : 1);

                if(strcmp(pString, sData) == 0) {
                    return (pCurrent - m_pData);
                }
            }

            if(iCurrent == '\0') {
                i++;
            }

            pCurrent++;
        }

        return -1;
    }

    size_t CStringPool::AddOrRef(const char* sData) {
        size_t iFind = this->Find(sData);
        if(iFind != -1) {
            return iFind;
        }

        return this->PushString(sData);
    }

    CStringPool::~CStringPool() {
        if(m_pData != NULL) {
            Free(m_pData);
        }
    }
}
