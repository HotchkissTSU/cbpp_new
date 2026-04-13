#ifndef CBPP_BINARY_CONV_H
#define CBPP_BINARY_CONV_H

#include <stdint.h>
#include <stddef.h>

namespace cbpp {
    enum class EBinaryClass : uint32_t {
        Plain,
        Array,
        BinTable,
        Image,
        String
    };

    /*
        Don`t use any unreliable-sized types to store data, e.g. CArray does store it`s
        size as uint64_t and not as size_t to maintain 32-bit compatibility and because
        i hate when value`s size changes randomly in my hallowed byte-fucking language.

        size_t is only acceptable in run-time since we can`t force the CPU to magically
        change it`s word size.
    */

    class IBinaryConvertible {
        public:
            /*
                Convert a class to the binary data.
                Pass NULL as target buffer to calculate it`s needed size
            */
            virtual size_t AsBinary(uint8_t* pBuffer) const = 0;

            /*
                Serialize a class from binary data.
            */
            virtual bool FromBinary(const uint8_t* pData, size_t iLength) = 0;
            virtual EBinaryClass GetBinaryClass() const = 0;
    };
}

#endif
