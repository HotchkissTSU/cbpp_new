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
}

#endif
