#ifndef CBPP_ENTITYPOOL_H
#define CBPP_ENTITYPOOL_H

#include "cbpp/cbdef.h"
#include "cbpp/entity/CBaseEntity.h"

/*
    The thing that manages entities storage.
    CBPP server do only have one pool, whereas clients have two
    separate pools - one for server-managed IDs (managed pool), 
    and other for completely-clientside IDs (sealed pool).

    Clientside ID`s are seamless between sealed and managed pools
    (managed ID`s are offset forward about sealed pool`s size)
*/

#ifdef CBPP_CLIENTSIDE
    // The size of our local sealed ID`s pool
    #define CBPP_SEALEDPOOL_SIZE 512
#endif

// The size of managed ID`s pool
#define CBPP_MANAGEDPOOL_SIZE 2048

namespace cbpp {

    // Entities storage manager
    class CEntityPool {
        friend CEntityPool* GetEntityPool();

        CBaseEntity* m_aManagedPool = NULL;
        CBPP_ON_CLIENT(
            CBaseEntity* m_aSealedPool = NULL;
        )

        CEntityPool(size_t iManaged, size_t iSealed);

        public:
            CEntityPool() = delete;
            CEntityPool(const CEntityPool& Other) = delete;
            CEntityPool& operator=(const CEntityPool& Other) = delete;

            // Attempt emplacing this entity
            eid_t EmplaceEntity(CBaseEntity* eCandidate);

            ~CEntityPool();
    };

    CEntityPool* GetEntityPool();
}

#endif
