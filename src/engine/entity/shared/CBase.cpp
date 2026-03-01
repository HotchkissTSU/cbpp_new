#include "engine/entity/CBase.h"

namespace cbpp::ent {
    CBase* CBase::__get_this() { return this; }

    CBase::properties_t& CBase::GetProperties() { return m_aProperties; }

    const char* CBase::Classname() { return "CBase"; }
}
