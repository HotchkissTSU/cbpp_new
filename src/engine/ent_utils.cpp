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

    CEntityRegistrator::CEntityRegistrator(const char* sClassname, factory_t fpConstr, datadesc_factory_t fpDataConstr, initfunc_t fpInit) {
        GetEntFactoryMap().Insert(sClassname, {fpConstr, fpDataConstr, fpInit});
    }

    ent::CBase* CreateEntityClass(const char* sClassname) {
        EntityRegistryInfo* Info = GetEntFactoryMap().At(sClassname);

        if(Info == NULL) {
            WriteLogf(ELogLevel::Error, "Attempt to create an unregistered entity of class '%s'", sClassname);
            return NULL;
        }

        if(Info->fpEntityCreator == NULL) {
            WriteLogf(ELogLevel::Error, "Attempt to create an abstract entity class '%s'", sClassname);
            return NULL;
        }

        return (*Info->fpEntityCreator)();
    }

    void InitEntity(ent::CBase* pEnt, void* pDatadesc) {
        EntityRegistryInfo* Info = GetEntFactoryMap().At(pEnt->Classname());
        Info->fpInitFunc(pEnt, pDatadesc);
    }

    void InitEntity(ent::CBase* pEnt, const char* sClassname, void* pDatadesc) {
        EntityRegistryInfo* Info = GetEntFactoryMap().At(sClassname);
        Info->fpInitFunc(pEnt, pDatadesc);
    }

    IEntityDatadesc* CreateEntityDatadesc(const char* sClassname) {
        EntityRegistryInfo* Info = GetEntFactoryMap().At(sClassname);

        if(Info == NULL) {
            WriteLogf(ELogLevel::Error, "Attempt to create a datadesc of the unregistered entity class '%s'", sClassname);
            return NULL;
        }

        if(Info->fpDatadescCreator == NULL) {
            WriteLogf(ELogLevel::Error, "Entity class '%s' somehow has no datadesc available", sClassname);
            return NULL;
        }

        return (*Info->fpDatadescCreator)();
    }
}
