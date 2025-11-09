#ifndef CBPP_MEMORY_H
#define CBPP_MEMORY_H

#include <stddef.h>

#include "cbpp/malloc_wrapper.h"

namespace cbpp {
    /*
        Heap-allocated array abstraction
        Does nothing with its contents
    */
    template <typename T> class CMemory {
        protected:
            T* m_pData = NULL;
            size_t m_iDataLength = 0;

        public:
            CMemory() = default;

            CMemory(size_t iSize) {
                m_pData = Malloc<T>(iSize);
                m_iDataLength = iSize;
            }

            CMemory(const T* pInitialData, size_t iLength) {
                m_pData = Malloc<T>(iLength);
                m_iDataLength = iLength;
                memcpy(m_pData, pInitialData, iLength * sizeof(T));
            }

            CMemory& operator=(const CMemory& Other) {
                const size_t iOtherLen = Other.Length();
                m_pData = Malloc<T>(iOtherLen);
                m_iDataLength = iOtherLen;
                memcpy(m_pData, Other.m_pData, iOtherLen * sizeof(T));
            }

            size_t Length() const { return m_iDataLength; }

            void Resize(size_t iNewLength) {
                m_pData = Realloc<T>(m_pData, iNewLength);
            }

            T* Pointer() { return m_pData; }

            ~CMemory() {
                if(m_pData != NULL) {
                    Free(m_pData);
                }
            }
    };
}

#endif
