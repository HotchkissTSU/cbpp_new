#include "cbpp/Bit.h"

namespace cbpp {
    bool HasZeroBits(uint64_t iNum) {
        return (iNum - 0x01010101) & (~iNum) & 0x80808080UL;
    }
}
