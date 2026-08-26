#ifndef BITUM_STRING_H
#define BITUM_STRING_H

#include "EngineAllocator.h"
#include "hlib/String.h"

namespace bitum {
    using string_t = hlib::CString<engine_alloc_t, &g_Allocator>;
}

#endif
