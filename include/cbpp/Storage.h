#ifndef CBPP_API_STORAGE_H
#define CBPP_API_STORAGE_H

#include <stdint.h>

#include "cbpp/Bit.h"

namespace cbpp {
    /*
        Storage array with static size
    */
    template <typename value_t, size_t t_iSize> class CStorage {
        struct BlockInfo {
            uint64_t m_iMask = UINT64_MAX;
        };

        
    };
}

#endif
