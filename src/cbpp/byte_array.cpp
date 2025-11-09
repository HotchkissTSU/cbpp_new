#include "cbpp/byte_array.h"

namespace cbpp {
    void CByteArray::PushBytes(const void* pBytes, size_t iLength) {
        const size_t iOldLength = m_iDataLength;
        CMemory::Resize(iOldLength + iLength);
        memcpy(&m_pData[iOldLength-1], pBytes, iLength);
    }
}
