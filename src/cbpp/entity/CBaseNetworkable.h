#ifndef CBPP_ENTITY_NETWORKABLE_H
#define CBPP_ENTITY_NETWORKABLE_H

#include "cbpp/entity/CBaseEntity.h"

/*
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
        for a specific side, should throw static assertions.
*/

namespace cbpp {
    /*
        A basis for all entities, which can be synced between the server and a client
    */
    class CBaseNetworkable : public CBaseEntity {
        public:
            CbNoProperties

            /*
                SERVERSIDE - send a sync package to the client
                CLIENTSIDE - receive aforementioned package and apply it
            */
            virtual void NetSync() = 0;

            virtual ~CBaseNetworkable() = 0;
    };
    CbAbstractEntity(CBaseNetworkable, "base_net");
}

#endif
