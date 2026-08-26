#ifndef BITUM_VECTOR_API_H
#define BITUM_VECTOR_API_H

#include <stddef.h>
#include <stdint.h>
#include <math.h>

#include "Constants.h"

namespace bitum {
    template <typename num_t, size_t t_iLength>
    class CVector {
        static_assert(t_iLength > 0, "Vector must have at least 1 component");

        num_t m_aData[ t_iLength ];

        using this_t = CVector<num_t, t_iLength>;

        public:
            CVector() = default;

            constexpr CVector(num_t fill) {
                for( size_t i = 0; i < t_iLength; i++ ) {
                    m_aData[i] = fill;
                }
            }

            template <typename... args_t>
            constexpr CVector(args_t... args) : m_aData{ ((num_t)args)... } {
                static_assert(sizeof...(args) == t_iLength, "Wrong number of vector arguments");
            }

            constexpr num_t& operator[](size_t index) {
                return m_aData[index];
            }

            constexpr const num_t& operator[](size_t index) const {
                return m_aData[index];
            }

            // X Y Z W

            constexpr num_t& x() {
                return m_aData[0];
            }

            constexpr const num_t& x() const {
                return m_aData[0];
            }

            constexpr num_t& y() {
                static_assert(t_iLength > 1, "This vector does not have the Y component");
                return m_aData[1];
            }

            constexpr const num_t& y() const {
                static_assert(t_iLength > 1, "This vector does not have the Y component");
                return m_aData[1];
            }

            constexpr num_t& z() {
                static_assert(t_iLength > 2, "This vector does not have the Z component");
                return m_aData[2];
            }

            constexpr const num_t& z() const {
                static_assert(t_iLength > 2, "This vector does not have the Z component");
                return m_aData[2];
            }

            constexpr num_t& w() {
                static_assert(t_iLength > 3, "This vector does not have the W component");
                return m_aData[3];
            }

            constexpr const num_t& w() const {
                static_assert(t_iLength > 3, "This vector does not have the W component");
                return m_aData[3];
            }

            // +

            template <typename other_num_t>
            constexpr this_t operator+( const CVector<other_num_t, t_iLength>& other ) const {
                this_t out;
                for( size_t i = 0; i < t_iLength; i++ ) {
                    out[i] = m_aData[i] + other[i];
                }

                return out;
            }

            template <typename other_num_t>
            constexpr this_t operator+( other_num_t other ) const {
                this_t out;
                for( size_t i = 0; i < t_iLength; i++ ) {
                    out[i] = m_aData[i] + other;
                }

                return out;
            }

            template <typename other_num_t>
            constexpr const this_t& operator+=( other_num_t other ) const {
                for( size_t i = 0; i < t_iLength; i++ ) {
                    m_aData[i] += other;
                }

                return *this;
            }

            // -

            template <typename other_num_t>
            constexpr this_t operator-( const CVector<other_num_t, t_iLength>& other ) const {
                this_t out;
                for( size_t i = 0; i < t_iLength; i++ ) {
                    out[i] = m_aData[i] - other[i];
                }

                return out;
            }
            
            template <typename other_num_t>
            constexpr this_t operator-( other_num_t other ) const {
                this_t out;
                for( size_t i = 0; i < t_iLength; i++ ) {
                    out[i] = m_aData[i] - other;
                }

                return out;
            }
            
            template <typename other_num_t>
            constexpr const this_t& operator-=( other_num_t other ) const {
                for( size_t i = 0; i < t_iLength; i++ ) {
                    m_aData[i] -= other;
                }

                return *this;
            }

            // *

            template <typename other_num_t>
            constexpr this_t operator*( const CVector<other_num_t, t_iLength>& other ) const {
                this_t out;
                for( size_t i = 0; i < t_iLength; i++ ) {
                    out[i] = m_aData[i] * other[i];
                }

                return out;
            }

            template <typename other_num_t>
            constexpr this_t operator*( other_num_t other ) const {
                this_t out;
                for( size_t i = 0; i < t_iLength; i++ ) {
                    out[i] = m_aData[i] * other;
                }

                return out;
            }

            template <typename other_num_t>
            constexpr const this_t& operator*=( other_num_t other ) const {
                for( size_t i = 0; i < t_iLength; i++ ) {
                    m_aData[i] *= other;
                }

                return *this;
            }

            constexpr const this_t& operator-() const {
                for(size_t i = 0; i < t_iLength; i++) {
                    m_aData[i] = -m_aData[i];
                }

                return *this;
            }

            // /

            template <typename other_num_t>
            constexpr this_t operator/( const CVector<other_num_t, t_iLength>& other ) const {
                this_t out;
                for( size_t i = 0; i < t_iLength; i++ ) {
                    out[i] = m_aData[i] / (other[i] + EPSILON);
                    // if only there was a way of disabling INF and NAN generation completely
                }

                return out;
            }
            
            template <typename other_num_t>
            constexpr this_t operator/( other_num_t other ) const {
                this_t out;
                for( size_t i = 0; i < t_iLength; i++ ) {
                    out[i] = m_aData[i] / (other + EPSILON);
                }

                return out;
            }

            template <typename other_num_t>
            constexpr const this_t& operator/=( other_num_t other ) const {
                for( size_t i = 0; i < t_iLength; i++ ) {
                    m_aData[i] /= (other + EPSILON);
                }

                return *this;
            }

            // products

            template <typename other_num_t>
            constexpr float_t DotProduct( const CVector<other_num_t, t_iLength>& other ) const {
                float_t out = 0;

                for( size_t i = 0; i < t_iLength; i++ ) {
                    out = out + m_aData[i] * other[i];
                }

                return out;
            }

            template <typename other_num_t>
            constexpr float_t CrossProduct( const CVector<other_num_t, t_iLength>& other ) const {
                static_assert(t_iLength == 3, "Cross product is implemented only for 3D vectors");

                return (m_aData[1] * other[2] -
                        m_aData[2] * other[1],

                        m_aData[2] * other[0] -
                        m_aData[0] * other[2],

                        m_aData[0] * other[1] -
                        m_aData[1] * other[0]);
            }

            constexpr float_t Length() const {
                float_t out = 0;

                for( size_t i = 0; i < t_iLength; i++ ) {
                    out = out + m_aData[i]*m_aData[i];
                }

                return sqrt((float_t)(out));
            }  

            constexpr float_t LengthSquared() const {
                float_t out = 0;

                for( size_t i = 0; i < t_iLength; i++ ) {
                    out = out + m_aData[i]*m_aData[i];
                }

                return out;
            }
    };

    using Vec2f = CVector<float_t, 2>;
    using Vec2i = CVector<int32_t, 2>;

    using Vec3f = CVector<float_t, 3>;
    using Vec3i = CVector<int32_t, 3>;
}

#endif
