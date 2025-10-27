#ifndef CBPP_LIST_H
#define CBPP_LIST_H

#include "cbpp/malloc_wrapper.h"

namespace cbpp {
    size_t CArray_CalculateLength(size_t iAlloc, size_t iSize);

    // CBPP growable array
    template <typename T> class CArray {
        public:
            CArray() = default;

            CArray(const CArray<T>& aOther) {
                m_iAllocated = aOther.m_iAllocated;
                m_iSize = aOther.m_iSize;

                m_pMemory = Realloc<T>(m_pMemory, m_iAllocated);
                for(size_t i = 0; i < m_iSize; i++) {
                    m_pMemory[i] = aOther.m_pMemory[i];
                }
            }

            CArray(CArray<T>&& Other) {
                m_iAllocated = Other.m_iAllocated;
                m_iSize = Other.m_iSize;
                m_pMemory = Other.m_pMemory;
            }
            
            CArray(const T* pSource, size_t iSrcLen) : m_iSize(iSrcLen) {
                m_iAllocated = iSrcLen;
                m_pMemory = Malloc<T>(m_iAllocated);
                memcpy(m_pMemory, pSource, sizeof(T) * iSrcLen);
            }

            CArray(size_t iInitialSize) : m_iSize(0) {
                m_iAllocated = iInitialSize;
                m_pMemory = Calloc<T>(m_iAllocated);
            }

            T& At(size_t iIndex) {
                #ifdef CBPP_DEBUG
                if(iIndex >= m_iSize) {
                    CbThrowErrorf("CArray index %lu is out of bounds (%lu)", iIndex, m_iSize);
                }
                #endif

                return m_pMemory[iIndex];
            }
            
            T const& At(size_t iIndex) const {
                #ifdef CBPP_DEBUG
                if(iIndex >= m_iSize) {
                    CbThrowErrorf("CArray index %lu is out of bounds (%lu)", iIndex, m_iSize);
                }
                #endif

                return const_cast<T const&>(m_pMemory[iIndex]);
            }

            T& operator[] (size_t iIndex) {
                #ifdef CBPP_DEBUG
                if(iIndex >= m_iSize) {
                    CbThrowErrorf("CArray index %lu is out of bounds (%lu)", iIndex, m_iSize);
                }
                #endif

                return m_pMemory[iIndex];
            }

            T const& operator[] (size_t iIndex) const {
                #ifdef CBPP_DEBUG
                if(iIndex >= m_iSize) {
                    CbThrowErrorf("CArray index %lu is out of bounds (%lu)", iIndex, m_iSize);
                }
                #endif

                return const_cast<T const&>(m_pMemory[iIndex]);
            }
            
            void Shrink() {
                m_pMemory = Realloc<T>(m_pMemory, m_iSize);
                m_iAllocated = m_iSize;
            }

            void Clear() {
                for(size_t i = 0; i < m_iSize; i++) {
                    m_pMemory[i].~T();
                }

                m_iSize = 0;
            }

            // Simply allocate a new place without invoking a copy operator
            // You MUST fill it with any valid data manually, or our destructor will perform a sexy and loud segfault
            size_t  PushEmpty() {
                m_iSize += 1;
                const size_t iNewAllocSize = CArray_CalculateLength(m_iAllocated, m_iSize);

                if(iNewAllocSize != m_iAllocated) {
                    m_iAllocated = iNewAllocSize;
                    m_pMemory = Realloc<T>(m_pMemory, m_iAllocated);
                }

                memset(&m_pMemory[m_iSize - 1], 0, sizeof(T));
                
                return m_iSize - 1;
            }
            
            size_t PushBack(T& Value) {
                m_iSize += 1;
                const size_t iNewAllocSize = CArray_CalculateLength(m_iAllocated, m_iSize);

                if(iNewAllocSize != m_iAllocated) {
                    m_iAllocated = iNewAllocSize;
                    m_pMemory = Realloc<T>(m_pMemory, m_iAllocated);
                }

                memset(&m_pMemory[m_iSize - 1], 0, sizeof(T));
                m_pMemory[m_iSize - 1] = Value;
                return m_iSize - 1;
            }

            size_t PushBack(T Value) {
                m_iSize += 1;
                const size_t iNewAllocSize = CArray_CalculateLength(m_iAllocated, m_iSize);

                if(iNewAllocSize != m_iAllocated) {
                    m_iAllocated = iNewAllocSize;
                    m_pMemory = Realloc<T>(m_pMemory, m_iAllocated);
                }

                memset(&m_pMemory[m_iSize - 1], 0, sizeof(T));
                m_pMemory[m_iSize - 1] = Value;
                return m_iSize - 1;
            }

            /*
                Locate COMPARE and replace it with VALUE, or push VALUE at the end of the array
                COMPARE and VALUE must have a defined comparison operator between each other
            */
            template <typename NULL_T> size_t Emplace(T& Value, const NULL_T& Compare) {
                for(size_t i = 0; i < m_iSize; i++) {
                    if(Value == Compare) {
                        m_pMemory[i] = Value;
                        return i;
                    }
                }

                return this->PushBack(Value);
            }
            
            /*size_t PushBack(T&& Value) {
                m_iSize += 1;
                const size_t iNewAllocSize = CArray_CalculateLength(m_iAllocated, m_iSize);

                if(iNewAllocSize != m_iAllocated) {
                    m_iAllocated = iNewAllocSize;
                    m_pMemory = Realloc<T>(m_pMemory, m_iAllocated);
                }

                memset(&m_pMemory[m_iSize - 1], 0, sizeof(T));
                m_pMemory[m_iSize - 1] = Value;
                return m_iSize - 1;
            }*/

            void PopBack() {
                m_pMemory[m_iSize-1].~T();
                m_iSize--;
            }

            void operator=(const CArray<T>& aOther) {
                this->Clear();
                m_iAllocated = aOther.m_iAllocated;
                m_iSize = aOther.m_iSize;

                m_pMemory = Realloc<T>(m_pMemory, m_iAllocated);
                for(size_t i = 0; i < m_iSize; i++) {
                    m_pMemory[i] = aOther.m_pMemory[i];
                }
            }

            T* Array() noexcept {
                return m_pMemory;
            }

            const T* Array() const noexcept {
                return const_cast<const T*> (m_pMemory);
            }

            size_t Find(const T& Target) const noexcept {
                for(size_t i = 0; i < m_iSize; i++) {
                    if( m_pMemory[i] == Target ) {
                        return i;
                    }
                }

                return -1;
            }
            
            // This array`s current length
            size_t Length() const {
                return m_iSize;
            }

            // The amount of allocated memory
            size_t Allocated() const {
                return m_iAllocated;
            }

            ~CArray() {
                if(m_pMemory != NULL) {
                    this->Clear();
                    Free(m_pMemory);
                }
            }

        protected:
            T* m_pMemory = NULL;
            size_t m_iAllocated = 0, m_iSize = 0;
    };
}

#endif
