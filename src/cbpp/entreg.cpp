#include "cbpp_api/Entity.h"
#include "cbpp/entity/CBaseEntity.h"

namespace cbpp {
    CBaseEntity* CreateEntityClass(const char* sClassName) {
        return CEntityRegistrator::GetFactoryPointer(sClassName)();
    }
}
