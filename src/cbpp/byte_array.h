#ifndef CBPP_BYTE_ARRAY_H
#define CBPP_BYTE_ARRAY_H

#include <stddef.h>

#include "cbpp/memory.h"

namespace cbpp {
    typedef unsigned char byte_t;

    /*
        Byte array abstraction
    */
    class CByteArray : protected CMemory<byte_t> {
        public:
            void PushBytes(const void* pBytes, size_t iCount);
    };
}

#endif
