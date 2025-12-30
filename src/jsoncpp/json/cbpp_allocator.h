#ifndef CBPP_JSON_ALLOCATOR_H
#define CBPP_JSON_ALLOCATOR_H

/*
    Replacement for the JsonCPP allocator
*/

#include "jsoncpp/json/allocator.h"
#include "cbpp/malloc_wrapper.h"

namespace Json {
    template <typename T> class CbppAllocator : public SecureAllocator<T> {
        public:
            pointer allocate(size_type iCount) {
                return cbpp::Malloc<T>(iCount);
            }

            void deallocate(pointer pData, size_type iCount) {
                memset(pData, 0, sizeof(T)*iCount);
                cbpp::Free(pData);
            }
    };
}

#endif
