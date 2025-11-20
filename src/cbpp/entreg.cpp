#include "cbpp/ent_register.h"
#include "cbpp/entity/CBaseEntity.h"
#include "cbpp_api/Entity.h"

namespace cbpp {
    CBaseEntity* CreateEntityClass(const char* sClassName) {
        return CEntityRegistrator::GetFactoryPointer(sClassName)();
    }
}
