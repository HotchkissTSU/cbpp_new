#ifndef CBPP_ENTITY_PROPERTY_H
#define CBPP_ENTITY_PROPERTY_H

#include <initializer_list>

#include "cbpp/Table.h"
#include "cbpp/String.h"

namespace cbpp {
    namespace ent { class CBase; }

    class IEntityProperty {};

    class CEntityPropsConstructor {
        public:
            CEntityPropsConstructor(void* pParent, std::initializer_list<IEntityProperty*> aProps);
    };

    class CEntityRegistrator {
        public:
            typedef ent::CBase* (*factory_t)(void); 
            CEntityRegistrator(const char* sClassname, factory_t fpConstr);
    };

    CBinTable<CConstString, CEntityRegistrator::factory_t>& GetEntFactoryMap();

    #define CbProperties(...)\
    virtual const char* Classname();\
    CEntityPropsConstructor __cb_props = { (void*)(__get_this()), {__VA_ARGS__} };

    #define CbRegisterEntity(_classname)                                \
        const char* _classname::Classname() { return #_classname; }     \
        cbpp::ent::CBase* __cb_entfact_##_classname() {                 \
            _classname* pEnt = cbpp::New<_classname>();                 \
            pEnt->GetProperties().Shrink();                             \
            return (cbpp::ent::CBase*)(pEnt);                           \
        }\
        static cbpp::CEntityRegistrator __cb_entreg_##_classname( #_classname, __cb_entfact_##_classname );
};

#endif
