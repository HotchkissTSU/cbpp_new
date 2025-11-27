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
        Any pointer to any member should be considered invalidated after any insertion.
    */
    template <typename key_t, typename value_t> class CTable {
        struct Pair {
            key_t Key;
            value_t Value;

            Pair(const key_t& _Key, const value_t& _Value) : Key(_Key), Value(_Value) {}
        };

        CArray<Pair> m_aData;

        size_t BinarySearch(const key_t& Key, bool& bSuccess) {
            size_t iFirst = 0, iLast = m_aData.Length()-1;
            size_t iMid = 0;

            while( iFirst < iLast ) {
                iMid = (iFirst + iLast) / 2;
                if(m_aData[iMid].Key == Key) {
                    bSuccess = true;
                    return iMid;
                }else if(m_aData[iMid].Key > Key) {
                    iLast = iMid;
                }else {
                    iFirst = iMid + 1;
                }
            }

            bSuccess = false;
            return iMid;
        }
        
        value_t& AccessRW(const key_t& Key) {
            if(m_aData.Length() == 0) { // first pair
                m_aData.PushEmpty();
                m_aData[0].Key = Key;
                return m_aData[0].Value;
            }

            Pair& Last = m_aData.Last();
            if(Key > Last.Key) {
                m_aData.PushEmpty();
                m_aData.Last().Key = Key;
                return m_aData.Last().Value;
            }

            bool bRes;
            size_t iIndex = BinarySearch(Key, bRes);

            if(bRes) { // this entry do already exist
                return m_aData[iIndex].Value;
            }

            // insertion!

            m_aData.PushEmpty();

            // shift everyone to the right to empty iIndex slot
            for(size_t i = m_aData.Length()-1; i > iIndex; i--) {
                memcpy( &m_aData[i], &m_aData[i-1], sizeof(Pair) );
            }

            m_aData[iIndex].Key = Key;
            memset(&m_aData[iIndex].Value, 0, sizeof(value_t));
            return m_aData[iIndex].Value;
        }
        
        value_t* AccessR(const key_t& Key) {
            bool bHasKey;
            size_t iIndex = BinarySearch(Key, bHasKey);

            if(bHasKey) {
                return &m_aData[iIndex].Value;
            }else {
                return NULL;
            }
        }
        
        public:
            // Access a member, or create a new insertion if there is no such an entry
            value_t& operator[](const key_t& Key) {
                return AccessRW(Key);
            }

            // Access a member, or create a new insertion if there is no such an entry
            const value_t& operator[](const key_t& Key) const {
                return const_cast<const value_t&>(AccessRW(Key));
            }

            // Access a member, only if it exists, otherwise NULL is returned
            value_t* At(const key_t& Key) {
                return AccessR(Key);
            }

            // Access a member, only if it exists, otherwise NULL is returned
            const value_t* At(const key_t& Key) const {
                return const_cast<const value_t*>(AccessR(Key));
            }

            bool HasKey(const key_t& Key) const {
                bool bResult;
                size_t iIndex = BinarySearch(Key, bResult);
                return bResult;
            }

            CArray<const key_t*> GetKeys() const {
                CArray<const key_t*> aOut(m_aData.Length());

                for(size_t i = 0; i < m_aData.Length(); i++) {
                    aOut[i] = const_cast<const key_t*>(&m_aData[i].Key);
                }

                return aOut;
            }

            void Shrink() {
                m_aData.Shrink();
            }
    };
}

#endif
