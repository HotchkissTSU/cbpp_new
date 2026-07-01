#include "cbpp/Memory.h"

#include "cbpp/Constants.h"

#include <stdlib.h>

#ifdef CBPP_LINUX
    #include <unistd.h>
#endif

#ifdef CBPP_WINDOWS
    #include <windows.h>
#endif

namespace cbpp {
    size_t& GetMallocCounter() {
        static size_t s_iMallocCounter = 0;
        return s_iMallocCounter;
    }

    void Free(void* pMemory) {
        GetAllocatorData().fpFree(pMemory); 
        GetMallocCounter()--;
    }

    size_t GetPageSize() {
        #if defined(CBPP_LINUX)

            long p = sysconf(_SC_PAGE_SIZE);
            if (p < 0) p = 4096;
            return (size_t)p;

        #elif defined(CBPP_WINDOWS)

            SYSTEM_INFO si;
            GetSystemInfo(&si);
            return (size_t)si.dwAllocationGranularity;

        #else
            #error "Unsupported platform"
        #endif
    }
}
