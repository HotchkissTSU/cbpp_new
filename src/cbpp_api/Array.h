#ifndef CBPP_LIST_H
#define CBPP_LIST_H

#include <new>

#include "cbpp/malloc_wrapper.h"

namespace cbpp {
    /*
        CB++ growable array
    */
    template <typename T> class CArray {
        public:
            CArray() = default;
            
            CArray(const CArray<T>& aOther) : m_iAllocated(aOther.m_iSize), m_iSize(aOther.m_iSize) {
                if (m_iSize > 0) {
                    m_pMemory = Malloc<T>(m_iSize);
                    for (size_t i = 0; i < m_iSize; ++i) {
                        new (&m_pMemory[i]) T(aOther.m_pMemory[i]);
                    }
                }
            }
            
            CArray(CArray<T>&& Other)  
                : m_pMemory(Other.m_pMemory), m_iAllocated(Other.m_iAllocated), m_iSize(Other.m_iSize) {
                Other.m_pMemory = NULL;
                Other.m_iAllocated = 0;
                Other.m_iSize = 0;
            }
            
            CArray(const T* pSource, size_t iSrcLen) : m_iSize(iSrcLen), m_iAllocated(iSrcLen) {
                if (iSrcLen > 0) {
                    m_pMemory = Malloc<T>(iSrcLen);
                    for (size_t i = 0; i < iSrcLen; ++i) {
                        new (&m_pMemory[i]) T(pSource[i]);
                    }
                }
            }
            
            CArray(size_t iInitialCapacity) : m_iSize(0), m_iAllocated(iInitialCapacity) {
                if (iInitialCapacity != 0) {
                    m_pMemory = Malloc<T>(iInitialCapacity);
                }
            }
            
            ~CArray() {
                Clear();
                if (m_pMemory != NULL) {
                    Free(m_pMemory);
                }
            }
            
            CArray& operator=(const CArray<T>& aOther) {
                Clear();
                
                m_pMemory = Realloc<T>(m_pMemory, aOther.m_iAllocated);

                m_iAllocated = aOther.m_iAllocated;
                m_iSize = aOther.m_iSize;

                for (size_t i = 0; i < m_iSize; ++i) {
                    m_pMemory[i] = aOther.m_pMemory[i];
                }
                
                return *this;
            }
            
            CArray& operator=(CArray<T>&& Other) {
                if (this != &Other) {
                    Clear();
                    if (m_pMemory != NULL) {
                        Free(m_pMemory);
                    }
                    
                    m_pMemory = Other.m_pMemory;
                    m_iAllocated = Other.m_iAllocated;
                    m_iSize = Other.m_iSize;
                    
                    Other.m_pMemory = NULL;
                    Other.m_iAllocated = 0;
                    Other.m_iSize = 0;
                }
                return *this;
            }
            
            T& At(size_t iIndex) {
                CbAssertf(iIndex >= m_iSize, "CArray index %lu is out of bounds (%lu)", iIndex, m_iSize);
                return m_pMemory[iIndex];
            }
            
            const T& At(size_t iIndex) const {
                CbAssertf(iIndex >= m_iSize, "CArray index %lu is out of bounds (%lu)", iIndex, m_iSize);
                return m_pMemory[iIndex];
            }
            
            T& operator[](size_t iIndex) { return At(iIndex); }
            const T& operator[](size_t iIndex) const { return At(iIndex); }
            
            T& Last() {
                CbAssert(m_iSize == 0, "List is empty");
                return m_pMemory[m_iSize - 1];
            }
            
            const T& Last() const {
                CbAssert(m_iSize == 0, "List is empty");
                return m_pMemory[m_iSize - 1];
            }
            
            void Shrink() {
                if(m_pMemory == NULL) { return; }
                m_pMemory = Realloc<T>(m_pMemory, m_iSize);
            }
            
            void Clear() {
                for (size_t i = 0; i < m_iSize; ++i) {
                    m_pMemory[i].~T();
                }
                m_iSize = 0;
            }
            
            size_t PushBack(const T& Value) {
                if(m_iSize >= m_iAllocated) {
                    Grow();
                }

                new(&m_pMemory[m_iSize]) T(Value);

                return m_iSize++;
            }
            
            size_t PushBack(T&& Value) {
                if(m_iSize >= m_iAllocated) {
                    Grow();
                }

                if constexpr( std::is_constructible_v<T, T&&> ) {
                    new(&m_pMemory[m_iSize]) T(std::move(Value));
                } else {
                    memcpy(&m_pMemory[m_iSize], &Value, sizeof(Value));
                }

                memset(&Value, 0, sizeof(Value)); // Reset the moved object

                return m_iSize++;
            }

            size_t PushEmpty() {
                if (m_iSize >= m_iAllocated) {
                    Grow();
                }
                new (&m_pMemory[m_iSize]) T();
                return m_iSize++;
            }
            
            void PopBack() {
                if (m_iSize > 0) {
                    m_pMemory[m_iSize - 1].~T();
                    --m_iSize;
                }
            }
            
            void InsertAt(size_t iPos, T&& Value) {
                CbAssert(iPos > m_iSize, "Out of bounds");

                if(m_iSize >= m_iAllocated) {
                    Grow();
                }

                if(iPos > m_iSize) { return; }

                m_iSize++;

                if(m_iSize > 0) {
                    // Shift everyone to free said index
                    for(size_t i = m_iSize - 1; i > iPos; --i) {
                        memcpy(&m_pMemory[i], &m_pMemory[i-1], sizeof(T));
                    }
                }

                // Move the value
                memcpy(&m_pMemory[iPos], &Value, sizeof(T));

                memset(&Value, 0, sizeof(Value)); // Reset the original object we have moved
            }
            
            void InsertAt(size_t iPos, const T& Value) {
                CbAssert(iPos > m_iSize, "Out of bounds");

                if(m_iSize >= m_iAllocated) {
                    Grow();
                }

                if(iPos > m_iSize) { return; }

                if(m_iSize > 0) {
                    for(size_t i = m_iSize - 1; i > iPos; --i) {
                        memcpy(&m_pMemory[i], &m_pMemory[i-1], sizeof(T));
                    }
                }

                m_iSize++;

                if constexpr(std::is_constructible_v<T, const T&>) {
                    new(&m_pMemory[iPos]) T(Value);
                } else {
                    memcpy(&m_pMemory[iPos], &Value, sizeof(Value));
                }
            }
            
            void RemoveAt(size_t iPos) {
                CbAssert(iPos >= m_iSize, "Out of bounds");
                
                m_pMemory[iPos].~T();

                for (size_t i = iPos; i + 1 < m_iSize; ++i) {
                    memcpy(&m_pMemory[i], &m_pMemory[i+1], sizeof(T));
                    m_pMemory[i + 1].~T();
                }
                
                --m_iSize;
            }
            
            size_t Find(const T& Target) const {
                for (size_t i = 0; i < m_iSize; ++i) {
                    if (m_pMemory[i] == Target) {
                        return i;
                    }
                }
                return (size_t)(-1);
            }
            
            T* Data() { return m_pMemory; }
            const T* Data() const { return m_pMemory; }
            
            size_t Length() const { return m_iSize; }
            size_t Capacity() const { return m_iAllocated; }
            
            void Reserve(size_t iNewCapacity) {
                if (iNewCapacity > m_iAllocated) {
                    m_pMemory = Realloc<T>(m_pMemory, iNewCapacity);
                    m_iAllocated = iNewCapacity;
                }
            }
            
        private:
            T* m_pMemory = NULL;
            size_t m_iAllocated = 0;
            size_t m_iSize = 0;
            
            void Grow() {
                size_t iNewAllocated = m_iAllocated == 0 ? 1 : m_iAllocated * 2;
                Reserve(iNewAllocated);
            }
    };
}

#endif
