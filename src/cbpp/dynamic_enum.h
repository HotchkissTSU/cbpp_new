#ifndef CBPP_DYNAMIC_ENUM_H
#define CBPP_DYNAMIC_ENUM_H

#include <stdint.h>
#include <stddef.h>

#include "cbpp/const_string.h"

namespace cbpp {
    template <typename T = int32_t> class CDynamicEnum {
        struct Pair {
            CConstString sName;
            T iValue;
        };

        public:
            template <typename... values_t> CDynamicEnum(values_t&&... Vals) {
                const size_t iNumArgs = sizeof...(Vals);
            }
    };
}

#endif
