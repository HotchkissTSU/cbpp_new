#ifndef CBPP_CDF_OBJECT_H
#define CBPP_CDF_OBJECT_H

#include <stddef.h>
#include <stdint.h>

#include "cbpp/cbdef.h"
#include "cbpp/malloc_wrapper.h"

namespace cbpp::cdf {
    enum class EObjectClass : int32_t {
        Object,
        Array,

        Number,
        Binary,

        LAST
    };

    class IObject {
        public:
            // Full size needed to store this object
            virtual size_t Sizeof() const = 0;

            // Length of essential data
            virtual size_t Length() const = 0;
            virtual EObjectClass Class() const = 0;

            virtual char* Data() = 0;

            virtual void PushData(const char* pData, size_t iDataSize) = 0;

            virtual ~IObject() = 0;
    };

    class CObjectStorage : public IObject {
        private:
            union Conv {
                char* Pointer;
                char Bytes[sizeof(char*)];
            };

            char* m_pData = NULL;
            int32_t m_iLength = 0;
            EObjectClass m_iClass;

        public:
            CObjectStorage(EObjectClass iClass, size_t iAlloc) {
                CbAssert(iAlloc == 0, "Attempt to allocate an object with zero memory");

                m_iClass = iClass;
                m_iLength = iAlloc;
                
                if(iAlloc <= 8) {
                    m_pData = NULL;
                } else {
                    m_pData = Calloc<char>(iAlloc);
                }
            }

            char* Data() override { return (char*)m_pData; }
            size_t Sizeof() const override { return sizeof(CObjectStorage) + m_iLength*(m_iLength > 8); }
            size_t Length() const override { return m_iLength; }
            EObjectClass Class() const override { return m_iClass; }

            bool IsSmallStorage() { return m_iLength <= 8; }

            void PushData(const char* pData, size_t iDataSize) {
                const size_t iNewSize = iDataSize + m_iLength;

                if(m_iLength >= 8) { // Regular pushback
                    m_pData = Realloc<char>(m_pData, iNewSize);
                    memcpy(m_pData + m_iLength, pData, iDataSize);
                    m_iLength = iNewSize;
                    return;

                } else if(iNewSize > 8 && m_iLength < 8) { // New size is bigger than 8 bytes, gonna switch to the external storage
                    char* pTemp;
                    memcpy(&pTemp, &m_pData, sizeof(pTemp));

                    m_pData = Malloc<char>(iNewSize);
                    memcpy(m_pData, &pTemp, m_iLength);
                    memcpy(m_pData + m_iLength, pData, iDataSize);
                    m_iLength = iNewSize;

                    return;

                } else { // Internal pushback
                    Conv Conv;
                    Conv.Pointer = m_pData;

                    memcpy(Conv.Bytes + m_iLength, pData, iDataSize);
                    memcpy(m_pData, &Conv, sizeof(Conv));
                    m_iLength = iNewSize;

                    return;
                }
            }
            
            virtual ~CObjectStorage() override {
                if(m_iLength > 8 && m_pData != NULL) {
                    Free(m_pData);
                }
            }
    };
}

#endif
