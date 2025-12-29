#include "cbpp/ent_register.h"
#include "cbpp/entity/CBaseEntity.h"
#include "cbpp_api/Entity.h"

namespace cbpp {
    CBaseEntity* CreateEntityClass(const char* sClassName) {
        return CEntityRegistrator::GetFactoryPointer(sClassName)();
    }

    entmap_t& GetEntityFactories() {
        static entmap_t s_dEntityDict;
        return s_dEntityDict;
    }

    CEntityRegistrator::CEntityRegistrator(const char* sClassName, entfact_t fpFactory) {
        /*
            Entity registrator objects are static, so there is a possibility that they will
            duplicate among different compiler translation units. This isn`t very bad, but
            we better avoid unneccecary data overwriting.
        */
        if(!GetEntityFactories().HasKey(sClassName)){
            GetEntityFactories()[sClassName] = fpFactory;
        }
    }
    
    CEntityRegistrator::entfact_t CEntityRegistrator::GetFactoryPointer(const char* sClassName) {
        entfact_t* fpFactory = GetEntityFactories().At(sClassName);
        CbAssertf( fpFactory == NULL, "Unregistered entity class: '%s'", sClassName );
        return *fpFactory;
    }
}
