#ifndef CBPP_ENTITY_BASE_H
#define CBPP_ENTITY_BASE_H

#include <stddef.h>
#include <stdint.h>

#include "cbpp/String.h"

#include "engine/property_interface.h"

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

            const char* Name() const;

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
