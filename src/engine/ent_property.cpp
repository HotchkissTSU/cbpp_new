#include "engine/ent_property.h"

#include "cbpp/Math.h"

namespace cbpp {
    const char* IEntityProperty::Name() const { return m_sName; }
    IEntityProperty::EType IEntityProperty::Type() const { return m_iType; }

    void CNumberEntityProperty::SetValue(int32_t iValue) {
        *m_pData = Clamp(iValue, m_iMin, m_iMax);
    }
    
    int32_t CNumberEntityProperty::GetValue() { return *m_pData; }

    Vec2i CNumberEntityProperty::Limits() { return {m_iMin, m_iMax}; }

    void CFloatEntityProperty::SetValue(float iValue) {
        *m_pData = Clamp(iValue, m_fMin, m_fMax);
    }

    float CFloatEntityProperty::GetValue() { return *m_pData; }

    Vec2f CFloatEntityProperty::Limits() { return {m_fMin, m_fMax}; }
}
