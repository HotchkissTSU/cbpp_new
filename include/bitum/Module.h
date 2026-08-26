#ifndef BITUM_MODULE_API
#define BITUM_MODULE_API

#include "containers/Table.h"
#include "containers/Array.h"
#include "hlib/String.h"

#include "Entity.h"

namespace bitum {
    struct SEntityInfo {
        IEntity*        (*fpCreate)(IEntityData*);
        IEntityData*    (*fpGetData)();
    };

    using CEntityTable = table_t<hlib::CConstString, SEntityInfo>;

    class IModule {
        public:
            virtual const char* Name() const = 0;

            virtual const CEntityTable& GetEntityTable() const = 0;
    };

    bool LoadModule(IModule*);
}

#endif
