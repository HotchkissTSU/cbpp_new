/*
    About CB++ entity system

    Every entity class has a hidden subclass called "Datadesc", which derives from entities
    parent own subclass, up to CBase.

    This means that every datadesc inherits parent entities dynamic properties.

    Upon creation, the entity receives it`s datadesc with any values that you may want to pass there.
    Use this info for initial setup. All values can be directly accessed with the -> operator.

    Think about Datadesc as of a table of settings which can be dynamically edited in runtime and then 
    automagically fed to the correct entity class.
*/

#ifndef CBPP_ENTITY_API_H
#define CBPP_ENTITY_API_H

#include "engine/entity/CBase.h"
#include "engine/ent_utils.h"

namespace cbpp {
    // Allocate an entity of the specified type
    ent::CBase* CreateEntityClass(const char* sClassname);

    // Allocate a datadesc class for the specified entity type
    IEntityDatadesc* CreateEntityDatadesc(const char* sClassname);

    void InitEntity(ent::CBase* pEnt, void* pDatadesc);
}

#endif
