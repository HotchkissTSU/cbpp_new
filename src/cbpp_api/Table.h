/*
    A simple array-based associative table with log2(N) access complexity

    Best for rare insertions and frequent reads.
*/

#ifndef CBPP_BINARY_MAP_H
#define CBPP_BINARY_MAP_H

#include "cbpp_api/Array.h"
#include "cbpp_api/Math.h"

namespace cbpp {
    template <typename key_t, typename value_t> class CBinTable {
        struct Pair {
            key_t Key;
            value_t Value;
            
            Pair() = default;
            Pair(const key_t& k, const value_t& v) : Key(k), Value(v) {}
            Pair(key_t&& k, value_t&& v) : Key(std::move(k)), Value(std::move(v)) {}
            
            bool operator==(const Pair& other) const { return Key == other.Key; }
            bool operator<(const Pair& other) const { return Key < other.Key; }
        };
        
        CArray<Pair> m_aData;
        
        int64_t BinarySearch(const key_t& Key) const {
            if (m_aData.Length() == 0) {
                return -1;  // -(0 + 1) = -1
            }
            
            int64_t iL = 0;
            int64_t iR = (int64_t)(m_aData.Length()) - 1;
            
            while (iL <= iR) {
                int64_t iM = iL + (iR - iL) / 2;
                const key_t& midKey = m_aData[iM].Key;
                
                if (midKey == Key) {
                    return iM;
                } else if (midKey < Key) {
                    iL = iM + 1;
                } else {
                    iR = iM - 1;
                }
            }
        
            return -iL - 1;
        }
        
        public:
            CBinTable() = default;

            const CArray<Pair>& Data() {
                return m_aData;
            }
            
            void Insert(const key_t& Key, const value_t& Value) {
                if (m_aData.Size() == 0) {
                    m_aData.EmplaceBack(Key, Value);
                    return;
                }
                
                int64_t iPos = BinarySearch(Key);
                
                if (iPos >= 0) {
                    m_aData[(size_t)iPos].Value = Value;
                } else {
                    size_t iInsertPos = (size_t)(-iPos - 1);
                    m_aData.InsertAt(iInsertPos, Key, Value);
                }
            }
            
            void Insert(key_t&& Key, value_t&& Value) {
                if (m_aData.Size() == 0) {
                    m_aData.EmplaceBack(std::move(Key), std::move(Value));
                    return;
                }
                
                int64_t iPos = BinarySearch(Key);
                
                if (iPos >= 0) {
                    m_aData[(size_t)iPos].Value = std::move(Value);
                } else {
                    size_t iInsertPos = (size_t)(-iPos - 1);
                    m_aData.InsertAt(iInsertPos, std::move(Key), std::move(Value));
                }
            }

            bool HasKey(const key_t& Key) const {
                return BinarySearch(Key) >= 0;
            }
            
            value_t* At(const key_t& Key) {
                int64_t iPos = BinarySearch(Key);
                if (iPos >= 0) {
                    return &m_aData[(size_t)iPos].Value;
                }
                return NULL;
            }
            
            const value_t* At(const key_t& Key) const {
                int64_t iPos = BinarySearch(Key);
                if (iPos >= 0) {
                    return &m_aData[(size_t)iPos].Value;
                }
                return NULL;
            }
            
            value_t& operator[](const key_t& Key) {
                int64_t iPos = BinarySearch(Key);
                
                if (iPos >= 0) {
                    return m_aData[(size_t)iPos].Value;
                } else {
                    size_t iInsertPos = (size_t)(-iPos - 1);
                    value_t DefaultValue{};
                    m_aData.InsertAt(iInsertPos, Key, std::move(DefaultValue));
                    return m_aData[iInsertPos].Value;
                }
            }
            
            bool Erase(const key_t& Key) {
                int64_t iPos = BinarySearch(Key);
                if (iPos >= 0) {
                    m_aData.RemoveAt((size_t)iPos);
                    return true;
                }
                return false;
            }

            void Clear() {
                m_aData.Clear();
            }

            void Shrink() {
                m_aData.Shrink();
            }

            size_t Size() const noexcept {
                return m_aData.Size();
            }
            
            size_t Length() const noexcept {
                return m_aData.Size();
            }

            void Reserve(size_t iCapacity) {
                m_aData.Reserve(iCapacity);
            }
            
            void Print() const {
                for (size_t i = 0; i < m_aData.Length(); ++i) {
                    printf("Pair[%i]: K '%s' V '%s'\n", i, m_aData[i].Key.String(), m_aData[i].Value.String());
                }
            }
    };
}

#endif
