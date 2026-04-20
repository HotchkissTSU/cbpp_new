#ifndef CBPP_API_STORAGE_H
#define CBPP_API_STORAGE_H

#include <stdint.h>

#include "cbpp/Bit.h"
#include "cbpp/Array.h"

namespace cbpp {

    /*
        An array with occupation awareness - unlike 
        CArray, this one can have holes
    */
    template <typename value_t> class CStorage {
        enum class ENodeState {
            Free = 1,               // free nodes are marked 0 to allow fast checks for free ones with ! operator
            Busy = 0
        };

        struct Block {
            uint64_t m_iMask = UINT64_MAX;
            char m_aData[sizeof(uint64_t)*8*sizeof(value_t)];

            bool HasFreeSpace() { return !m_iMask; }

            int FindFreeIndex() {
                if( !this->HasFreeSpace() ) { return -1; }

                const int iBits = 8 * sizeof(m_iMask);
                for(int i = 0; i < iBits; i++) {
                    if( GetBit(m_iMask, i) == (bool)ENodeState::Free ) {
                        return i;
                    }
                }

                return -1;
            }

            void SetIndexState(int iIndex, ENodeState iState) {
                SetBit(m_iMask, iIndex, (bool)iState);
            }

            value_t* operator[](int iIndex) {
                return (value_t*)(m_aData + iIndex*sizeof(value_t));
            }
        };

        CArray<Block> m_aBlocks;

        public:
            CStorage() = default;

            CStorage(size_t iInitialSize) {
                const size_t iBlocks = iInitialSize / sizeof(uint64_t) * 8 + 1;

                m_aBlocks.Reserve(iBlocks);

                Block Buff;
                Buff.m_iMask = (uint64_t)(-1);
                memset(Buff.m_aData, 0, sizeof(Buff.m_aData));

                for(size_t i = 0; i < iBlocks; i++) {
                    m_aBlocks.PushBack(Buff);
                }
            }

            value_t* Allocate() {
                for(size_t i = 0; i < m_aBlocks.Length(); i++) {
                    Block& Current = m_aBlocks[i];

                    if(Current.HasFreeSpace()) {
                        int iIndex = Current.FindFreeIndex();

                        return Current[iIndex];

                    } else {
                        continue;
                    }
                }

                m_aBlocks.PushEmpty();

                Block& Last = m_aBlocks.Last();
                Last.m_iMask = (uint64_t)(-1) & (~-1); // set first index as occupied
                memset(Last.m_aData, 0, sizeof(Last.m_aData));

                return Last[0];
            }
    };
}

#endif
