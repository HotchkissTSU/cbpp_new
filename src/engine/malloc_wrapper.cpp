#include "cbpp/Memory.h"

#include <stdlib.h>

namespace cbpp {
    size_t& GetMallocCounter() {
        static size_t s_iMallocCounter = 0;
        return s_iMallocCounter;
    }

    void Free(void* pMemory) {
        GetAllocatorData().fpFree(pMemory); 
        GetMallocCounter()--;
    }
}
