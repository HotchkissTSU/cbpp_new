#include "engine/ent_utils.h"

#include "engine/entity/CBase.h"
#include "cbpp/Error.h"

namespace cbpp {
    CEntityPropsConstructor::CEntityPropsConstructor(void* pParent, std::initializer_list<IEntityProperty*> aProps) {
        for(IEntityProperty* pCurrent : aProps) {
            ((ent::CBase*)pParent)->GetProperties().PushBack(pCurrent);
        }
    }

    CBinTable<CConstString, CEntityRegistrator::factory_t>& GetEntFactoryMap() {
        static CBinTable<CConstString, CEntityRegistrator::factory_t> s_dFacts;
        return s_dFacts;
    }

    CEntityRegistrator::CEntityRegistrator(const char* sClassname, factory_t fpConstr) {
        GetEntFactoryMap().Insert(sClassname, fpConstr);
    }

    ent::CBase* CreateEntityClass(const char* sClassname) {
        CEntityRegistrator::factory_t* pFactory = GetEntFactoryMap().At(sClassname);
        if(pFactory == NULL) {
            WriteLogf(ELogLevel::Error, "Attempt to create an unregistered entity of class '%s'", sClassname);
            return NULL;
        }

        return (*pFactory)();
    }
}
