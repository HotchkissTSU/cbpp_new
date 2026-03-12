#include "engine/entity/CBase.h"

namespace cbpp::ent {
    CBase::~CBase() {}

    void CBase::Construct() {puts("CBase construct!");}
    void CBase::Destruct() {}

    void CBase::Init(Datadesc*) {puts("CBase init!");}

    CBase::Datadesc* CBase::Datadesc::__get_this() {
        return this;
    }

    size_t CBase::Datadesc::Length() {
        return m_aProps.Length();
    }

    IEntityProperty* CBase::Datadesc::At(size_t iIndex) {
        return m_aProps[iIndex];
    }

    void CBase::Datadesc::PushProperty(IEntityProperty* pProp) {
        m_aProps.PushBack(pProp);
    }

    CBase::Datadesc::~Datadesc() {
        for(int i = 0; i < m_aProps.Length(); i++) {
            IEntityProperty* pCurrent = m_aProps[i];
            cbpp::Delete(pCurrent);
        }
    }
}
