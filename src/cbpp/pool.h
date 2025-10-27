#ifndef CBPP_POOL_H
#define CBPP_POOL_H

/*
    Pool allocator.
    Stores same objects in cache-friendly continuous blocks, and
    supports general pointer interface.
*/

#include <stdint.h>
#include <math.h>
#include <new>

#include "cbpp/bit.h"

#define pooled_class(_name, _startsize) class _name final : public cbpp::IPoolAllocated<_name>
#define pooled_struct(_name, _startsize) struct _name final : public cbpp::IPoolAllocated<_name>

namespace cbpp {
    /*
        Get an unsigned integer of the chosen type with its maximal value
        (All bytes are set to 0xFF)
    */
    template <typename UINT_T> constexpr UINT_T GetMaxUInteger() {
        UINT_T iOut;
        char* pOut = (char*)(&iOut);
        for(int i = 0; i < sizeof(UINT_T); i++) {
            pOut[i] = 0xff;
        }

        return iOut;
    }

    // Pool allocator slot state toggles
    enum ESlotState : uint8_t {
        ESlotState_BUSY = 0,
        ESlotState_FREE = 1
    };

    template <typename type_t, typename mask_t> struct PoolChunk {
        uint8_t m_aData[sizeof(mask_t)*8*sizeof(type_t)];
        mask_t m_iMask;

        PoolChunk<type_t, mask_t>* m_pNextChunk = NULL;

        /*
            Free/Occupied state flag is inverted to allow this hypersonic-fast free space check
        */

        bool HasFreeSpace() {
            return (bool)(m_iMask) == true;
        }

        size_t FindFreeIndex() {
            for( size_t i = 0; i < sizeof(mask_t)*8; i++ ) {
                if( GetBit(m_iMask, i) == ESlotState_FREE ) {
                    return i;
                }
            }

            return -1;
        }
        
        void MarkIndex(size_t iIndex, uint8_t bFlag) {
            SetBit(m_iMask, iIndex, bFlag);
        }

        /*
            Validate if this pointer belongs to this chunk
        */
        bool CheckPointer(type_t* pPtr) {
            if( !((size_t)pPtr >= (size_t)(&m_aData)) ) { // after the chunk beginning
                return false;
            }

            if( !((size_t)pPtr <= (m_aData + (sizeof(mask_t)*8 - 1)*sizeof(type_t))) ) { // before the chunk end minus one
                return false;
            }

            if( !(((size_t)pPtr - (size_t)(&m_aData)) % sizeof(type_t) == 0) ) { // does not point in the middle of a slot
                return false;
            }

            return true;
        }

        type_t* At(size_t iIndex) {
            return (type_t*)(&m_aData[iIndex*sizeof(type_t)]);
        }

        void Free() {
            for(size_t i = 0; i < sizeof(mask_t)*8; i++) {
                if( GetBit(m_iMask, i) == ESlotState_BUSY ) {
                    this->At(i)->~type_t();
                }
            }

            if( m_pNextChunk != NULL ) {
                m_pNextChunk->Free();
                cbpp::Free(m_pNextChunk);
            }
        }
    };

    template <typename type_t, typename mask_t = uint32_t> class Pool {
        typedef PoolChunk<type_t, mask_t> chunk_t;

        chunk_t* m_FirstChunk;

        public:
            Pool(const Pool<type_t, mask_t>& Other) = delete;
            void operator=(const Pool<type_t, mask_t>& Other) = delete;

            Pool() {
                m_FirstChunk = Malloc<chunk_t>();
                m_FirstChunk->m_iMask = GetMaxUInteger<mask_t>();
            }

            Pool(size_t iInitialLength) {
                m_FirstChunk = Malloc<chunk_t>();
                m_FirstChunk->m_iMask = GetMaxUInteger<mask_t>();

                const size_t iBlocksAmount = (size_t)( ceilf((float)iInitialLength / (float)(sizeof(mask_t)*8)) );

                chunk_t* pCurrent = m_FirstChunk;
                for(size_t i = 1; i < iBlocksAmount; i++) {
                    pCurrent->m_pNextChunk = Malloc<chunk_t>();
                    pCurrent->m_pNextChunk->m_iMask = GetMaxUInteger<mask_t>();
                    pCurrent = pCurrent->m_pNextChunk;
                }
            }

            chunk_t* PushBlock() {
                chunk_t* pCurrent = m_FirstChunk;
                forever {
                    if(pCurrent->m_pNextChunk == NULL) {
                        pCurrent->m_pNextChunk = Malloc<chunk_t>();
                        pCurrent->m_pNextChunk->m_iMask = GetMaxUInteger<mask_t>();
                        return pCurrent->m_pNextChunk;
                    } else {
                        pCurrent = pCurrent->m_pNextChunk;
                    }
                }
            }
            
            /*
                Convert an external pointer to the internal chunk+index pair
                Returns NULL if the pointer is not from this pool
            */
            chunk_t* PointerToIndex(size_t& iIndex, type_t* pPtr) {
                chunk_t* pCurrent = m_FirstChunk;
                while(pCurrent != NULL) {
                    if( pCurrent->CheckPointer(pPtr) ) {
                        iIndex = ((size_t)pPtr - (size_t)(&pCurrent->m_aData)) / sizeof(type_t);
                        return pCurrent;
                    }

                    pCurrent = pCurrent->m_pNextChunk;
                }

                return NULL;
            }

            /*
                Returns a pointer to a free slot, or NULL if no free space is left
            */
            type_t* FindFreeSpace(chunk_t*& pChunk, size_t& iIndex) {
                chunk_t* pCurrent = m_FirstChunk;
                while(pCurrent != NULL) {
                    if( pCurrent->HasFreeSpace() ) {
                        pChunk = pCurrent;
                        iIndex = pCurrent->FindFreeIndex();

                        return pCurrent->At(iIndex);
                    }

                    pCurrent = pCurrent->m_pNextChunk;
                }

                return NULL;
            }

            template <typename... args_t> type_t* Allocate(args_t... Args) {
                chunk_t* pChunk;
                size_t iIndex;

                type_t* pSlot = this->FindFreeSpace(pChunk, iIndex);
                if(pSlot == NULL) {
                    this->PushBlock();
                    pSlot = this->FindFreeSpace(pChunk, iIndex);
                }

                memset(pSlot, 0, sizeof(type_t));
                ConstructBuffer(pSlot, Args...);

                pChunk->MarkIndex(iIndex, ESlotState_BUSY);

                return pSlot;
            }

            void Free(type_t* pPtr) {
                size_t iIndex;
                chunk_t* pChunk = this->PointerToIndex(iIndex, pPtr);
                if(pChunk != NULL) {
                    pChunk->MarkIndex(iIndex, ESlotState_FREE);
                    pPtr->~type_t();
                }
            }

            void PrintInfo() {
                size_t iChunkCounter = 0;
                chunk_t* pCurrent = m_FirstChunk;
                while(pCurrent != NULL) {
                    printf("Chunk %zu: ", iChunkCounter);

                    PrintBits(pCurrent->m_iMask);

                    pCurrent = pCurrent->m_pNextChunk;
                    iChunkCounter++;
                }
            }

            ~Pool() {
                m_FirstChunk->Free();
                cbpp::Free(m_FirstChunk);
            }
    };

    template <typename type_t, size_t t_iInitialSize> Pool<type_t, uint32_t>* GetStaticPool() {
        static Pool<type_t, uint32_t> s_Pool(t_iInitialSize);
        return &s_Pool;
    }

    template <typename type_t, size_t t_iInitialize = 32> class IPoolAllocated {
        public:
            template <typename... args_t> static type_t* AllocatePooled(args_t... Args) {
                Pool<type_t, uint32_t>* pGlobalPool = GetStaticPool<type_t, t_iInitialize>();
                return pGlobalPool->Allocate(Args...);
            }
    };

    /*
        Allocate and initialize a class in the global pool
    */
    template <typename type_t, typename... args_t> type_t* Palloc(args_t... Args) {
        return type_t::AllocatePooled(Args...);
    }
}

#endif
