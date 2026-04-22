#include "cbpp/Stack.h"

namespace cbpp {
    CBinaryStack::CBinaryStack(const uint8_t* pData, size_t iLength) {
        m_pData = Malloc<uint8_t>(iLength);
        m_iSize = iLength;
        memcpy(m_pData, pData, iLength);
    }

    void CBinaryStack::Clear() {
        m_iSize = 0;
        Free(m_pData);
        m_pData = NULL;
    }

    const uint8_t* CBinaryStack::GetBuffer() const {
        return (const uint8_t*)m_pData;
    }

    uint8_t* CBinaryStack::GetBuffer() {
        return m_pData;
    }

    size_t CBinaryStack::Length() const {
        return m_iSize;
    }

    void CBinaryStack::PushBytes(const uint8_t* pData, size_t iLength) {
        size_t iOldSize = m_iSize;
        m_iSize += iLength;
        m_pData = Realloc(m_pData, m_iSize);
        memcpy(m_pData + iOldSize, pData, iLength);
    }

    size_t CBinaryStack::PopBytes(uint8_t* pBuffer, size_t iLength) {
        memcpy(pBuffer, m_pData + m_iSize - iLength, iLength);

        size_t iOldSize = m_iSize;
        
        if((m_iSize - iLength == 0) || (iLength > m_iSize)) {
            m_iSize = 1;
        } else {
            m_iSize = m_iSize - iLength;
        }

        m_pData = Realloc(m_pData, m_iSize);

        return iOldSize - m_iSize;
    }

    CBinaryStack::~CBinaryStack() {
        if(m_pData != NULL) {
            Free(m_pData);
            m_pData = NULL;
        }
    }
}
