#include "cbpp/Time.h"

#include <time.h>

#include "cbpp/Constants.h"

#ifdef CBPP_WINDOWS
    #include <windows.h>
#endif

namespace cbpp {
    uint64_t GetCPUTime() {
        #if defined(CBPP_LINUX)
            struct timespec ts;
            if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts) != 0) return -1;
            return (uint64_t)ts.tv_sec * 1000000000LL + ts.tv_nsec;
        #elif defined(CBPP_WINDOWS)
            FILETIME ftCreation, ftExit, ftKernel, ftUser;
            HANDLE h = GetCurrentProcess();
            GetProcessTimes(h, &ftCreation, &ftExit, &ftKernel, &ftUser)
            uint64_t k = ((uint64_t)ftKernel.dwHighDateTime << 32) | ftKernel.dwLowDateTime;
            uint64_t u = ((uint64_t)ftUser.dwHighDateTime   << 32) | ftUser.dwLowDateTime;
            // FILETIME units are 100-ns intervals -> convert to ns
            uint64_t total100ns = k + u;
            return (uint64_t)(total100ns * 100); // to nanoseconds
        #endif
    }

    uint64_t GetTime() {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts); // monotonic avoids jumps
        return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
    }
}
