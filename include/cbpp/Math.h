#ifndef CBPP_MATH_API_H
#define CBPP_MATH_API_H

#include "cbpp/Vector.h"

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

    // Is power of 2
    template <typename INT_T> bool IsPOT(INT_T iValue) {
        return (iValue > 0) && ((iValue & (iValue - 1)) == 0);
    }

    Vec2f VectorFromAngle(float fAng);
    float AngleFromVector(Vec2f vVec);

    size_t PlanarToLinear(size_t iSideLength, size_t iX, size_t iY);
    void LinearToPlanar(size_t iSideLength, size_t iLinear, size_t& iX, size_t& iY);

    uint32_t CeilToPowerOf2(uint32_t iV);
}

#endif
