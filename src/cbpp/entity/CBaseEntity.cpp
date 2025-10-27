#include "cbpp/entity/CBaseEntity.h"

#include <string.h>

#include "cbpp/vector.h"

namespace cbpp {
    const size_t g_aGTypesSize[] = {
        0,
        sizeof(int32_t),
        sizeof(int64_t),
        sizeof(float),
        sizeof(float)*2,
        sizeof(float)*3,
        sizeof(uint8_t)*4,
        0
    };

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
        CBPP_ASSERT( GetEntityFactories().count(sClassName) == 0, "Unregistered entity class" );
        return GetEntityFactories().at(sClassName);
    }

    CBaseEntity* CreateEntityClass(const char* sClassName) {
        return CEntityRegistrator::GetFactoryPointer(sClassName)();
    }

    IProperty* IProperty::Next() { return m_pNext; }
    void IProperty::SetNextNode(IProperty* pNext) { m_pNext = pNext; }

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

    const char* CBaseEntity::Class() const {
        return "base";
    }

    CBaseEntity::~CBaseEntity() {
        delete m_pFirstProp;
    }
}
