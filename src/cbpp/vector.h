#ifndef CBPP_VECTOR_H
#define CBPP_VECTOR_H

#include "cbpp/cbdef.h"

#include <math.h>
#include <string.h>
#include <stdint.h>

/*
    Note that almost all internal math is using floats
*/

namespace cbpp {
    template <typename member_t> class CVector {
        member_t m_x, m_y;

        public:
            template <typename number_t> CVector(number_t Value) {
                m_x = (member_t)(Value);
                m_y = (member_t)(Value);
            }

            template <typename number_t> CVector(number_t X, number_t Y) {
                m_x = (member_t)(X);
                m_y = (member_t)(Y);
            }

            template <typename numberA_t, typename numberB_t> CVector(numberA_t X, numberB_t Y) {
                m_x = (member_t)(X);
                m_y = (member_t)(Y);
            }

            CVector() {
                memset(this, 0, sizeof(CVector<member_t>)); // kaboom
            }
            
            member_t& X() { return m_x; }
            member_t& Y() { return m_y; }

            float Length() const {
                return sqrt((float)(m_x*m_x + m_y*m_y));
            }
            
            float LengthSquared() const {
                return (float)(m_x*m_x + m_y*m_y);
            }

            CVector<member_t> operator+(float Value) const {
                return CVector<member_t>(m_x + (member_t)Value, m_y + (member_t)Value);
            }

            CVector<member_t> operator-(float Value) const {
                return CVector<member_t>(m_x - (member_t)Value, m_y - (member_t)Value);
            }

            CVector<member_t> operator*(float Value) const {
                return CVector<member_t>(m_x * (member_t)Value, m_y * (member_t)Value);
            }

            CVector<member_t> operator/(float Value) const {
                return CVector<member_t>(m_x / ((member_t)Value + CBPP_EPSILON), m_y / ((member_t)Value + CBPP_EPSILON));
            }

            CVector<member_t> operator+(CVector<member_t> Other) const {
                return CVector<member_t>(m_x + Other.m_x, m_y + Other.m_y);
            }

            CVector<member_t> operator-(CVector<member_t> Other) const {
                return CVector<member_t>(m_x - Other.m_x, m_y - Other.m_y);
            }

            CVector<member_t> operator*(CVector<member_t> Other) const {
                return CVector<member_t>(m_x * Other.m_x, m_y * Other.m_y);
            }

            CVector<member_t> operator/(CVector<member_t> Other) const {
                // A dumb way of dealing with division on zero
                return CVector<member_t>(m_x / (Other.m_x + CBPP_EPSILON), m_y / (Other.m_y + CBPP_EPSILON));
            }

            CVector<member_t> Rotated(float fAngle) const {
                float fX = m_x*std::cos(fAngle) - m_y*std::sin(fAngle);
		        float fY = m_x*std::sin(fAngle) + m_y*std::cos(fAngle);
                return CVector<member_t>(fX, fY);
            }

            CVector<member_t> Normalized() const {
                float fLength = this->Length();
                float fX = (float)m_x / (fLength + CBPP_EPSILON);
                float fY = (float)m_y / (fLength + CBPP_EPSILON);
                return CVector<member_t>(fX, fY);
            }
    };

    typedef CVector<float> Vec2f;
    typedef CVector<double> Vec2d;
    typedef CVector<int32_t> Vec2i;
    typedef CVector<int64_t> Vec2l;
}

#endif
