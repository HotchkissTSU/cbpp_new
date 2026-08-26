#ifndef BITUM_TABLE_H
#define BITUM_TABLE_H

#include "EngineAllocator.h"
#include "hlib/Table.h"

namespace bitum {
    template <typename key_t, typename value_t>
    using table_t = hlib::CBinTable<key_t, value_t, engine_alloc_t, &g_Allocator>;
}

#endif
