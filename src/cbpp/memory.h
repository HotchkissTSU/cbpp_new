#ifndef CBPP_MEMORY_H
#define CBPP_MEMORY_H

#include <stddef.h>

#include "cbpp/malloc_wrapper.h"

namespace cbpp {
    /*
        Some allocated memory
    */
    template <typename T> class Memory {
        T* m_pData = NULL;
        size_t m_iDataLength = 0;

        public:
            // Default constructors are called
            Memory(size_t iLength) {
                m_pData = Malloc<T>(iLength);
                m_iDataLength = iLength;

                T* pCurrent = m_pData;
                for(size_t i = 0; i < iLength; i++) {
                    ConstructBuffer(pCurrent);
                    pCurrent++;
                }
            }
            
            // Copy constructors are called with the specific argument
            Memory(size_t iLength, T& Value) {
                m_pData = Malloc<T>(iLength);
                m_iDataLength = iLength;

                T* pCurrent = m_pData;
                for(size_t i = 0; i < iLength; i++) {
                    ConstructBuffer(pCurrent, Value);
                    pCurrent++;
                }
            }

            // General constructor with variadic parameters
            template <typename... args_t> Memory(size_t iLength, args_t... Args) {
                m_pData = Malloc<T>(iLength);
                m_iDataLength = iLength;

                T* pCurrent = m_pData;
                for(size_t i = 0; i < iLength; i++) {
                    ConstructBuffer(pCurrent, Args...);
                    pCurrent++;
                }
            }

            // Copy operators are called
            Memory( const Memory<T>& Other ) {
                const size_t iOtherLen = Other.Length();

                m_pData = Malloc<T>(iOtherLen);
                for(size_t i = 0; i < Other.Length(); i++) {
                    m_pData[i] = Other.Pointer()[i];
                }
            }
            
            // Indexing operator
            T& operator[] (size_t iIndex) {
                return m_pData[iIndex];
            }

            const T& operator[] (size_t iIndex) const {
                return const_cast<const T&>(this->operator[](iIndex));
            }

            size_t Length() const { return m_iDataLength; }

            /*
                Get the raw underlying pointer.
                PLEASE do not mess with him up.
            */
            T* Pointer() { return m_pData; }
            const T* Pointer() const { return const_cast<const T*>(m_pData); }

            ~Memory() {
                if(m_pData != NULL) { Free(m_pData); }
                m_iDataLength = 0;
            }
    };
}

#endif
