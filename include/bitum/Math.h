#ifndef BITUM_MATH_API_H
#define BITUM_MATH_API_H

#include "Constants.h"

namespace bitum::math {
    template <typename num_t>
    inline num_t Clamp(num_t x, num_t minv, num_t maxv) {
        return (x > maxv) ? maxv : ((x < minv) ? minv : x);
    }

    template <typename num_t, typename... args_t>
    inline num_t Max(num_t first, args_t... args) {
        num_t out = first;

        ((out = args > out ? (num_t)(args) : out), ...);

        return out;
    }

    template <typename num_t, typename... args_t>
    inline num_t Min(num_t first, args_t... args) {
        num_t out = first;

        ((out = args < out ? (num_t)(args) : out), ...);

        return out;
    }

    template <typename num_t>
    inline bool InRange(num_t x, num_t minv, num_t maxv) {
        return (x >= minv) && (x <= maxv);
    }

    template <typename num_t>
    inline bool InRangeStrict(num_t x, num_t minv, num_t maxv) {
        return (x > minv) && (x < maxv);
    }

    
}

#endif
