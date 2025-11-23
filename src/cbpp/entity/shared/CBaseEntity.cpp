#include "cbpp/entity/CBaseEntity.h"

#include <string.h>

#include "cbpp/vector.h"
#include "cbpp/ent_register.h"

namespace cbpp {    
    std::map<CConstString, CEntityRegistrator::entfact_t>& GetEntityFactories() {
        static std::map<CConstString, CEntityRegistrator::entfact_t> s_dEntityDict;
        return s_dEntityDict;
    }

    CEntityRegistrator::CEntityRegistrator(const char* sClassName, entfact_t fpFactory) {
        /*
            Entity registrator objects are static, so there is a possibility that they will
            duplicate among different compiler translation units. This isn`t very bad, but
            we better avoid unneccecary data overwriting.
        */
        if(GetEntityFactories().count(sClassName) == 0){
            GetEntityFactories()[sClassName] = fpFactory;
        }
    }

    CEntityRegistrator::entfact_t CEntityRegistrator::GetFactoryPointer(const char* sClassName) {
        CbAssertf( GetEntityFactories().count(sClassName) == 0, "Unregistered entity class: '%s'", sClassName );
        return GetEntityFactories().at(sClassName);
    }

    eid_t CBaseEntity::UID() const { return m_iUID; }
    void CBaseEntity::SetUID(eid_t iNewID) { m_iUID = iNewID; }

    IProperty* CBaseEntity::GetProperties() { return m_pFirstProp; }
    const IProperty* CBaseEntity::GetProperties() const { return const_cast<const IProperty*>(m_pFirstProp); }

    void CBaseEntity::PushProperty(IProperty* pProp) {
        if(m_pFirstProp == NULL) {
            m_pFirstProp = pProp;
            return;
        }

        IProperty* pCurrent = m_pFirstProp;
        while(pCurrent != NULL) {
            IProperty* pNext = pCurrent->Next();
            if( pNext == NULL ) {
                pCurrent->SetNextNode(pProp);
                return;
            } else {
                pCurrent = pNext;
            }
        }
    }

    IProperty* CBaseEntity::GetPropertyByName(const char* sName) {
        IProperty* pCurrent = m_pFirstProp;
        while(pCurrent != NULL) {
            if( strcmp(sName, pCurrent->Name()) == 0 ) {
                return pCurrent;
            }

            pCurrent = pCurrent->Next();
        }

        return NULL;
    }

    void CBaseEntity::Think() {
        printf("CBaseEntity thinker\n");
    }

    const char* CBaseEntity::Class() const {
        return "base";
    }

    bool CBaseEntity::IsAbstract() const {
        return true;
    }

    const char* CBaseEntity::Name() const {
        return m_sWorldName;
    }

    CBaseEntity::~CBaseEntity() {
        delete m_pFirstProp;
    }
}
