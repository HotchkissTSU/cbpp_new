#ifndef BITUM_STACK_T
#define BITUM_STACK_T

#include "EngineAllocator.h"
#include "hlib/Stack.h"

namespace bitum {
    template <typename value_t>
    using stack_t = hlib::CStack<value_t, engine_alloc_t, &g_Allocator>;
}

#endif
