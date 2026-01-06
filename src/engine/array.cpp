#include "cbpp/Array.h"

namespace cbpp {
    size_t CArray_CalculateLength(size_t iAlloc, size_t iSize) {
        if(iSize == 0) {
            return 1;
        }

        if(iSize < iAlloc) {
            return iAlloc;
        }

        return iSize*2;
    }
}
