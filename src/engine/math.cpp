#ifndef CBPP_MATH_H
#define CBPP_MATH_H

#include "cbpp/Math.h"

namespace cbpp {
    Vec2f VectorFromAngle(float fAng) {
        return Vec2f(std::cosf(fAng), std::sinf(fAng));
    }

    float AngleFromVector(Vec2f vVec) {
        return atan2f(vVec.Y(), vVec.X());
    }

    size_t PlanarToLinear(size_t iSideLength, size_t iX, size_t iY) {
        return iSideLength * iY + iX;
    }
    
    void LinearToPlanar(size_t iSideLength, size_t iLinear, size_t& iX, size_t& iY) {
        iX = iLinear % iSideLength;
        iY = iLinear / iSideLength;
    }

    uint32_t CeilToPowerOf2(uint32_t v) {
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;
        return v;
    }
}

#endif
