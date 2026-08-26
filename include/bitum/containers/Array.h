#ifndef BITUM_ARRAY_H
#define BITUM_ARRAY_H

#include "EngineAllocator.h"
#include "hlib/Array.h"

namespace bitum {
    template <typename value_t>
    using array_t = hlib::CArray<value_t, engine_alloc_t, &g_Allocator>;
}

#endif
