/*
    About CB++ entity system

    Every entity class has a hidden subclass called "Datadesc", which derives from entities
    parent own subclass, up to CBase.

    This means that every datadesc inherits parent entities dynamic properties.

    Upon creation, the entity receives it`s datadesc with any values that you may want to pass there.
    Use these info for initial setup. All values can be directly accessed with the -> operator.

    The setup data is completely separated from it`s entity class and does not affect it`s runtime life at all.
*/

#ifndef CBPP_ENTITY_API_H
#define CBPP_ENTITY_API_H

#include "engine/entity/CBase.h"
#include "engine/ent_utils.h"

namespace cbpp {
    // Allocate an entity of the specified type
    ent::CBase* CreateEntityClass(const char* sClassname, void* pData = NULL);

    // Allocate a datadesc class for the specified entity type
    IEntityDatadesc* CreateEntityDatadesc(const char* sClassname);
}

#endif
