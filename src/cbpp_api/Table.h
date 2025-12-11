/*
    A simple array-based associative table with log2(N) access complexity
*/

#ifndef CBPP_BINARY_MAP_H
#define CBPP_BINARY_MAP_H

#include "cbpp_api/Array.h"
#include "cbpp_api/Math.h"

namespace cbpp {
    /*
        Associative array based on binary search.
    */
    template<typename key_t, typename value_t> class CBinTable {
        struct Pair {
            key_t Key;
            value_t Value;
        };

        CArray<Pair> m_aData;
        
        int64_t BinarySearch(const key_t& Key) const {
            int64_t iL = 0;
            int64_t iR = m_aData.Length() - 1;
            
            while (iL <= iR) {
                // Безопасное вычисление середины, избегаем переполнения
                int64_t iM = iL + (iR - iL) / 2;
                
                if (m_aData[iM].Key == Key) {
                    return iM; // Точное совпадение
                } else if (m_aData[iM].Key < Key) {
                    iL = iM + 1;
                } else { // key < data[mid].first
                    iR = iM - 1;
                }
            }
            
            return -iL - 1; // No key was found, return the insertion index
        }
        
        public:
            CBinTable() = default;
            
            // Insert an element
            void Insert(const key_t& Key, const value_t& Value) {
                if(m_aData.Length() == 0) {
                    m_aData.PushBack( Pair{Key, Value} );
                    return;
                }

                int64_t iPos = BinarySearch(Key);
                
                if (iPos >= 0) {
                    m_aData[iPos].Value = Value;
                } else {
                    int64_t iInsertPos = -iPos - 1;

                    m_aData.PushEmpty();
                    
                    for (int64_t i = m_aData.Length() - 1; i > iInsertPos; --i) {
                        m_aData[i] = m_aData[i - 1];
                    }

                    m_aData[iInsertPos] = Pair {Key, Value};
                }
            }
            
            bool HasKey(const key_t& Key) const {
                return BinarySearch(Key) >= 0;
            }
            
            value_t* At(const key_t& Key) {
                int64_t iPos = BinarySearch(Key);
                if (iPos >= 0) {
                    return &m_aData[iPos].Value;
                }
                return NULL;
            }
            
            const value_t* At(const key_t& Key) const {
                int64_t iPos = BinarySearch(Key);
                if (iPos >= 0) {
                    return const_cast<const value_t*>(&m_aData[iPos].Value);
                }
                return NULL;
            }
            
            value_t& operator[](const key_t& Key) {
                if(m_aData.Length() == 0) {
                    m_aData.PushBack( Pair {Key, value_t()} );
                }

                int64_t iPos = BinarySearch(Key);

                if (iPos >= 0) {
                    return m_aData[iPos].Value;
                } else {
                    int64_t iInsertPos = -iPos - 1;

                    m_aData.PushEmpty();

                    for (int64_t i = m_aData.Length() - 1; i > iInsertPos; --i) {
                        m_aData[i] = std::move(m_aData[i - 1]);
                    }

                    m_aData[iInsertPos] = Pair {Key, value_t()};
                    return m_aData[iInsertPos].Value;
                }
            }
            
            size_t Length() const {
                return m_aData.Length();
            }
            
            bool Erase(const key_t& Key) {
                int64_t iPos = BinarySearch(Key);
                if (iPos >= 0) {
                    
                    for (size_t i = iPos; i < m_aData.Length() - 1; i++) {
                        m_aData[i] = std::move(m_aData[i + 1]);
                    }
                    m_aData.PopBack();

                    return true;
                }
                return false;
            }
            
            void Clear() {
                m_aData.Clear();
            }

            void Print() {
                for(size_t i = 0; i < m_aData.Length(); i++) {
                    printf("Pair [%d]: {'%s' = %f}\n", i, (const char*)(m_aData[i].Key), m_aData[i].Value);
                }
            }
    };
}

#endif
