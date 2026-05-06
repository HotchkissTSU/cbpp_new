#ifndef CBPP_API_TIME_H
#define CBPP_API_TIME_H

#include <stdint.h>

namespace cbpp {
    // Get the current CPU time in nanoseconds
    uint64_t GetCPUTime();

    // Get wall-clock time in nanoseconds
    uint64_t GetTime();
}

#endif
