#include "engine/entity/CBase.h"

namespace cbpp::ent {
    CBase::CBase(Datadesc* pData) {
        puts("CBase construction!");
    }

    const char* CBase::Classname() {
        return "CBase";
    }

    const char* CBase::Base() {
        return "CBPP";
    }

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
}
