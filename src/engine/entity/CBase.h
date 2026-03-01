#ifndef CBPP_ENTITY_BASE_H
#define CBPP_ENTITY_BASE_H

#include "cbpp/Array.h"
#include "cbpp/String.h"
#include "cbpp/Constants.h"

namespace cbpp { class IEntityProperty; }
namespace cbpp::ent {
    // A basis for all game entities
    class CBase {
        protected:
            CBase* __get_this();
            CArray<IEntityProperty*> m_aProperties;

            CString m_sWorldName; // Other entities can refer to us by this name

        public:
            typedef CArray<IEntityProperty*> properties_t;

            CBase() = default;
            CBPP_PROTECTED_CLASS(CBase)
            virtual ~CBase() = default;

            const char* Name();
            void SetName(const char* sName);

            virtual const char* Classname();

            properties_t& GetProperties();
    };
}

#endif
