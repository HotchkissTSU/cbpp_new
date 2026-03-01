#include "cbpp/Matrix.h"

namespace cbpp {
    CMatrix2::CMatrix2(float x00, float x01, float x10, float x11) {
        m_aData[0][0] = x00; m_aData[0][1] = x01;
        m_aData[1][0] = x10; m_aData[1][1] = x11;
    }

    float CMatrix2::Determinant() const {
        // A B
        // C D          D = AD-BC

        return m_aData[0][0] * m_aData[1][1] - m_aData[0][1] * m_aData[1][0];
    }

    CMatrix2 CMatrix2::operator*(const CMatrix2& Other) {
        // A B      E F         AE+BG AF+BH
        // C D      G H         CE+DG CF+DH

        const float A = m_aData[0][0],
                    B = m_aData[0][1],
                    C = m_aData[1][0],
                    D = m_aData[1][1];

        const float E = Other[0][0],
                    F = Other[0][1],
                    G = Other[1][0],
                    H = Other[1][1];

        return CMatrix2(
            A*E+B+G, A*F+B*H,
            C*E+D*G, C*F+D*H
        );
    }
}
