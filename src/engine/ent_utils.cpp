#include "engine/ent_utils.h"

#include "engine/entity/CBase.h"
#include "cbpp/Error.h"

namespace cbpp {
    CEntityPropsConstructor::CEntityPropsConstructor(void* pParent, std::initializer_list<IEntityProperty*> aProps) {
        for(IEntityProperty* pCurrent : aProps) {
            ((ent::CBase::Datadesc*)pParent)->PushProperty(pCurrent);
        }
    }

    CBinTable<CConstString, EntityRegistryInfo>& GetEntFactoryMap() {
        static CBinTable<CConstString, EntityRegistryInfo> s_dFacts;
        return s_dFacts;
    }

    CEntityRegistrator::CEntityRegistrator(const char* sClassname, factory_t fpConstr, datadesc_factory_t fpDataConstr) {
        GetEntFactoryMap().Insert(sClassname, {fpConstr, fpDataConstr});
    }

    ent::CBase* CreateEntityClass(const char* sClassname, void* pData) {
        EntityRegistryInfo* Info = GetEntFactoryMap().At(sClassname);

        if(Info == NULL) {
            WriteLogf(ELogLevel::Error, "Attempt to create an unregistered entity of class '%s'", sClassname);
            return NULL;
        }

        return (*Info->fpEntityCreator)(pData);
    }

    IEntityDatadesc* CreateEntityDatadesc(const char* sClassname) {
        EntityRegistryInfo* Info = GetEntFactoryMap().At(sClassname);

        if(Info == NULL) {
            WriteLogf(ELogLevel::Error, "Attempt to create a datadesc of the unregistered entity '%s'", sClassname);
            return NULL;
        }

        return (*Info->fpDatadescCreator)();
    }
}
