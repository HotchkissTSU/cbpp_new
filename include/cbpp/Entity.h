
/*
    CB++ engine entity API

    Here are some rules about CB++ networking design:

    1) Server is always true

    2) Clientside entities shall only perform rendering, input handling and some other non-important code,
        unless the entity is completely clientside.

    3) Vice versa, serverside entities do all the work: listen for client`s inputs, do thingies and then send themselves to
        clients for them to copy and interp to.

    4) Entity IDs must be synced between all clients, if the entity is not completely clientside. Otherwise, such entities must not collide
        with other server-managed IDs. Server does all of the entity creation/deletion stuff and then tells everybody via the net messages

    5) Clients only receive updates on entities inside their PVS. Getting in/out of the PVS should not be interpolated. 

    6) Clientside / serverside class interfaces should be as similar as possible in the sake of readability. Functions, which are restricted
        for a specific side, should throw assertions.
*/

#ifndef CBPP_ENTITY_API_H
#define CBPP_ENTITY_API_H

#include <stddef.h>

#include "engine/ent_props.h"
#include "engine/ent_register.h"
#include "engine/entity/CBaseEntity.h"

namespace cbpp {
    constexpr size_t GetGenericTypeSize(EGenericType iCode);

    /*
        Create an entity of this classname
    */
    CBaseEntity* CreateEntityClass(const char* sClassName);
}

#endif
