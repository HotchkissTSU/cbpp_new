#include "cbpp/ent_props.h"

#include "cbpp_api/Math.h"
#include "cbpp/entity/CBaseEntity.h"

namespace cbpp {
    IProperty::IProperty(CBaseEntity* eMaster, const char* sName) : m_eMaster(eMaster), m_sName(sName) {
        eMaster->PushProperty(this);
    }

    const char* IProperty::Name() {
        return m_sName;
    }

    const char* IProperty::PrettyName() {
        return m_sName;
    }

    const char* IProperty::Description() {
        return m_sName;
    }

    IProperty::~IProperty() {
        Free(m_pNext);
    }

    IProperty* IProperty::Next() { return m_pNext; }
    void IProperty::SetNextNode(IProperty* pNext) { m_pNext = pNext; }

    // CIntProperty

    void CIntProperty::SetValue(int32_t iValue) {
        (*m_pData) = Clamp(iValue, m_iMin, m_iMax);
    }

    int32_t CIntProperty::GetValue() {
        return *m_pData;
    }

    // CFloatProperty

    void CFloatProperty::SetValue(float iValue) {
        (*m_pData) = Clamp(iValue, m_fMin, m_fMax);
    }

    float CFloatProperty::GetValue() {
        return *m_pData;
    }

    // CEnumProperty

    void CEnumProperty::SetValue(uint16_t iValue) {
        *(m_pData) = iValue;
    }

    uint16_t CEnumProperty::GetValue() {
        return *m_pData;
    }

    const CEnumProperty::pairs_t& CEnumProperty::GetPairs() const {
        return m_aPairs;
    }
}
