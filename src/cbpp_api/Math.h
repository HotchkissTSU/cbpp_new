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

    Vec2f VectorFromAngle(float fAng) {
        fAng = fAng - CBPP_PI*2 * std::floor(fAng / CBPP_PI*2); // Normalize our angle
        const Vec2f vRaw( std::cos(fAng), std::sin(fAng) );

        if( InRange(fAng, 0.0f, CBPP_PI / 2) ) {                // First quarter
            return vRaw;
        } else if( InRange(fAng, CBPP_PI / 2, CBPP_PI) ) {      // Second quarter
            return vRaw * Vec2f(-1,1);
        } else if( InRange(fAng, CBPP_PI, 1.5f * CBPP_PI) ) {   // Third quarter
            return vRaw * Vec2f(-1);
        } else {
            return vRaw * Vec2f(1,-1);                          // And the fourth one
        }
    }
}

#endif
