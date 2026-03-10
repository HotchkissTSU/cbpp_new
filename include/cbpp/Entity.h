#ifndef CBPP_ENTITY_API_H
#define CBPP_ENTITY_API_H

#include "engine/entity/CBase.h"
#include "engine/ent_utils.h"

namespace cbpp {
    ent::CBase* CreateEntityClass(const char* sClassname, void* pData = NULL);
    IEntityDatadesc* CreateEntityDatadesc(const char* sClassname);
}

#endif
