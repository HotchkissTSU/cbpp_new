#ifndef CBPP_ENTITY_BASE_H
#define CBPP_ENTITY_BASE_H

#include <stddef.h>
#include <stdint.h>
#include <map>
#include <new>
#include <typeinfo>

#include "cbpp/cbdef.h"
#include "cbpp/const_string.h"

#include "cbpp_api/Entity.h"

namespace cbpp {
    class CBaseEntity;

    extern const size_t g_aGTypesSize[];

    template <typename type_t> class CProperty final : public IProperty {
        type_t* m_pData = NULL;
        size_t m_iSize;
        CBaseEntity* m_eMaster = NULL;
        EGenericType m_iType;
        const char *m_sName, *m_sDesc, *m_sPrettyName;

        public:
            CProperty(type_t* pData, CBaseEntity* eMaster, const char* sName, const char* sPrettyName, const char* sDesc, EGenericType iType, size_t iSize) : 
                    m_sName(sName), m_sDesc(sDesc), m_pData(pData), m_eMaster(eMaster), m_iType(iType), m_iSize(iSize), m_sPrettyName(sPrettyName)
            {
                CbAssert((pData == NULL) || (eMaster == NULL) || (sName) == NULL, "Invalid input data");
                eMaster->PushProperty((IProperty*)(this));
            }

            void* GetBuffer() { return (void*)(m_pData); }

            size_t Sizeof() { return sizeof(type_t); }
            const char* Name() { return m_sName; }
            const char* PrettyName() { return m_sPrettyName; }
            const char* Description() { return m_sDesc; }
            CBaseEntity* Master() { return m_eMaster; }

            EGenericType Type() {
                return m_iType;
            }

            void* Index(size_t iIndex) {
                if(iIndex >= m_iSize) {
                    return NULL;
                }

                return (void*)((size_t)m_pData + iIndex*g_aGTypesSize[(uint32_t)m_iType]);
            }

            size_t Length() {
                return m_iSize;
            }

            ~CProperty() {
                if(m_pNext != NULL) {
                    delete m_pNext;
                }
            }
    };
        
    class CEntityRegistrator {
        public:
            typedef CBaseEntity* (*entfact_t)();
            CEntityRegistrator(const char* sClassName, entfact_t fpFactory);

            static entfact_t GetFactoryPointer(const char* sClassName);
    };

    std::map<CConstString, CEntityRegistrator::entfact_t>& GetEntityFactories();
}

#endif
