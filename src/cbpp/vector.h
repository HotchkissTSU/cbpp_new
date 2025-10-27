#ifndef CBPP_CVector_H
#define CBPP_CVector_H

#include "cbpp/cbdef.h"

#include <math.h>
#include <string.h>
#include <stdint.h>

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
                memset(this, 0, sizeof(CVector<member_t>));
            }

            member_t& X() { return m_x; }
            member_t& Y() { return m_y; }

            CVector<member_t> operator+(CVector<member_t>& Other) {
                return CVector<member_t>(m_x + Other.m_x, m_y + Other.m_y);
            }

            CVector<member_t> operator-(CVector<member_t>& Other) {
                return CVector<member_t>(m_x - Other.m_x, m_y - Other.m_y);
            }

            CVector<member_t> operator*(CVector<member_t>& Other) {
                return CVector<member_t>(m_x * Other.m_x, m_y * Other.m_y);
            }

            CVector<member_t> operator/(CVector<member_t>& Other) {
                return CVector<member_t>(m_x / (Other.m_x + CBPP_EPSILON), m_y / (Other.m_y + CBPP_EPSILON));
            }

            float Length() {
                return sqrt((float)(m_x*m_x + m_y*m_y));
            }
            
            float LengthSquared() {
                return (float)(m_x*m_x + m_y*m_y);
            }

            template <typename number_t> CVector<member_t> operator+(number_t Value) {
                return CVector<member_t>(m_x + (member_t)Value, m_y + (member_t)Value);
            }

            template <typename number_t> CVector<member_t> operator-(number_t Value) {
                return CVector<member_t>(m_x - (member_t)Value, m_y - (member_t)Value);
            }

            template <typename number_t> CVector<member_t> operator*(number_t Value) {
                return CVector<member_t>(m_x * (member_t)Value, m_y * (member_t)Value);
            }

            template <typename number_t> CVector<member_t> operator/(number_t Value) {
                return CVector<member_t>(m_x / ((member_t)Value + CBPP_EPSILON), m_y / ((member_t)Value + CBPP_EPSILON));
            }

            CVector<member_t> Rotated(float fAngle) {
                float fX = m_x*std::cos(fAngle) - m_y*std::sin(fAngle);
		        float fY = m_x*std::sin(fAngle) + m_y*std::cos(fAngle);
                return CVector<member_t>(fX, fY);
            }

            CVector<member_t> Normalized() {
                float fLength = this->Length();
                float fX = (float)m_x / fLength;
                float fY = (float)m_y / fLength;
                return CVector<member_t>(fX, fY);
            }
    };

    typedef CVector<float> Vec2f;
    typedef CVector<double> Vec2d;
    typedef CVector<int32_t> Vec2i;
    typedef CVector<int64_t> Vec2l;
}

#endif
