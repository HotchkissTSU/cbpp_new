#ifndef CBPP_ENTITY_BASE_H
#define CBPP_ENTITY_BASE_H

#include "cbpp/Array.h"
#include "cbpp/String.h"
#include "cbpp/Constants.h"

#include "engine/ent_utils.h"

namespace cbpp { class IEntityProperty; }
namespace cbpp::ent {
    class CBase {
        public:
            class Datadesc : public IEntityDatadesc {
                private:
                    CEntityPropsConstructor m_Props = { __get_this(), {} };

                protected:
                    CArray<IEntityProperty*> m_aProps;
                    Datadesc* __get_this();

                public:
                    CString sWorldName;

                    size_t Length();
                    IEntityProperty* At(size_t iIndex);

                    void PushProperty(IEntityProperty* pProp);

                    virtual ~Datadesc() = default;
            };

            CBase() = delete;
            CBase(Datadesc* pData);

            virtual const char* Classname();
            virtual const char* Base();
    };
}

#endif
