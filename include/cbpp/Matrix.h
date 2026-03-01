#ifndef CBPP_MATRIX_H
#define CBPP_MATRIX_H

#include "cbpp/Vector.h"

namespace cbpp {
    template <size_t t_iRows, size_t t_iColumns, typename float_t> class CBaseMatrix {
        protected:
            float_t m_aData[t_iRows][t_iColumns];

        public:
            typedef CBaseMatrix <t_iRows, t_iColumns, float_t> matrix_t;

            struct Row {
                float_t* pRow;

                float_t& operator[](size_t iIndex) {
                    return pRow[iIndex];
                }
            };

            CBaseMatrix() = default;

            // Matrix mutliplication
            matrix_t operator*(const matrix_t& Other);

            // Matrix multiplication with a column-vector
            matrix_t operator*(float* pVector);

            // Multiply all elements by this factor
            matrix_t operator*(float Coeff);

            matrix_t operator/(float Coeff);

            matrix_t operator+(const matrix_t& Other);
            matrix_t operator-(const matrix_t& Other);

            float Determinant() const;

            matrix_t GetTransposed() const;

            Row operator[](size_t iIndex) const {
                Row Out;
                Out.pRow = (float*)&(m_aData[iIndex]);
                return Out;
            }
    };
    
    class CMatrix2 : public CBaseMatrix<2, 2, float> {
        public:
            typedef CMatrix2 matrix_t;

            CMatrix2(float x00, float x01, float x10, float x11);

            // Matrix mutliplication
            matrix_t operator*(const matrix_t& Other);

            float Determinant() const;

            matrix_t GetTransposed() const;
    };

    class CMatrix3 : public CBaseMatrix<3, 3, float> {
        
    };
}

#endif
