#include "engine/ent_property.h"

#include "cbpp/Math.h"

namespace cbpp {
    const char* IEntityProperty::Name() const { return m_sName; }
    IEntityProperty::EType IEntityProperty::Type() const { return m_iType; }

    const char* IEntityProperty::GetTypeName(IEntityProperty::EType iType) {
        switch (iType) {
            case IEntityProperty::EType::Integer:   return "Integer";
            case IEntityProperty::EType::Float:     return "Float";
            case IEntityProperty::EType::String:    return "String";
            case IEntityProperty::EType::Color:     return "Color";
            case IEntityProperty::EType::Vector:    return "Vector";
            case IEntityProperty::EType::Enum:      return "Enumeration";
            default:                                return "Unknown";
        }
    }

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

    void CStringEntityProperty::SetValue(const char* sData) {
        m_pData->Set(sData);
    }

    void CStringEntityProperty::SetValue(const CString& sData) {
        m_pData->Set(sData);
    }

    const char* CStringEntityProperty::GetValue() {
        return m_pData->String();
    }

    size_t CStringEntityProperty::Length() {
        return m_pData->Length();
    }

    void CVectorEntityProperty::SetValue(Vec2f vValue) {
        m_pData->X() = Clamp(vValue.X(), m_vMin.X(), m_vMax.X());
        m_pData->Y() = Clamp(vValue.Y(), m_vMin.Y(), m_vMax.Y());
    }

    Vec2f CVectorEntityProperty::GetValue() {
        return *m_pData;
    }

    Vec2f CVectorEntityProperty::MinBound() {
        return m_vMin;
    }

    Vec2f CVectorEntityProperty::MaxBound() {
        return m_vMax;
    }

    Color CColorEntityProperty::GetValue() {
        return *m_pData;
    }

    void CColorEntityProperty::SetValue(Color Data) {
        (*m_pData) = Data;
    }

    void CBoolEntityProperty::SetValue(bool bValue) {
        *m_pData = bValue;
    }

    bool CBoolEntityProperty::GetValue() {
        return *m_pData;
    }
}
