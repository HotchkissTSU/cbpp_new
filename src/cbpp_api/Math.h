#ifndef CBPP_MATH_API_H
#define CBPP_MATH_API_H

#include "cbpp/vector.h"

namespace cbpp {
    // Min <= V <= Max
    template <typename num_t> bool InRange(num_t V, num_t Min, num_t Max) {
        return (V >= Min) && (V <= Max);
    }

    // Min < V < Max
    template <typename num_t> bool InRangeStrict(num_t V, num_t Min, num_t Max) {
        return (V > Min) && (V < Max);
    }

    template <typename A, typename B, typename C> A Clamp(A Value, B Min, C Max) {
        if(Value > Max) {
            return Max;
        }

        if(Value < Min) {
            return Min;
        }

        return Value;
    }

    Vec2f VectorFromAngle(float fAng);
    float AngleFromVector(Vec2f vVec);
}

#endif
