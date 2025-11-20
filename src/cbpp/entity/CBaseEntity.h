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

#define CBPP_ENTMAP_SIZE 512
#define CBPP_DEFAULT_ENTITY_NAME "<UNNAMED>"

namespace cbpp {
    typedef uint32_t eid_t;

    class CBaseEntity {
        eid_t m_iUID;
        IProperty* m_pFirstProp = NULL;

        CString m_sWorldName = CBPP_DEFAULT_ENTITY_NAME;

        protected:
            void SetUID(eid_t iNewID);

        public:
            virtual void Think();
            virtual void Render() = 0;
            virtual void NetSync() = 0;

            virtual bool IsAbstract() const;
            virtual const char* Class() const;

            void PushProperty(IProperty* pProp);
            IProperty* GetProperties();
            const IProperty* GetProperties() const;
            IProperty* GetPropertyByName(const char* sName);

            eid_t UID() const;

            CBaseEntity() {};
            virtual ~CBaseEntity();
    };
}

#endif
