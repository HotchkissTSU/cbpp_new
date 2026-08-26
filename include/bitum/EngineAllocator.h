#ifndef BITUM_ALLOCATOR_H
#define BITUM_ALLOCATOR_H

#include "hlib/Allocator.h"

namespace bitum {
    using engine_alloc_t = hlib::CDefaultAllocator;
    extern engine_alloc_t g_Allocator;
};

#endif
