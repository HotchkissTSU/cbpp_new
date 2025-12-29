#ifndef CBPP_ENTREG_H
#define CBPP_ENTREG_H

#include "cbpp/const_string.h"
#include "cbpp_api/Table.h"

// Register an entity to allow its creation via the text label
#define CbRegisterEntity(cpp_class, classname)                                                                                  \
    cbpp::CBaseEntity* __entfact_##cpp_class() { cpp_class* eNew = cbpp::New<cpp_class>(); return eNew; }                       \
    static cbpp::CEntityRegistrator g_hEntityReg_##cpp_class(classname, __entfact_##cpp_class);                                 \
    const char* cpp_class::Class() const { return classname; }                                                                  \
    bool cpp_class::IsAbstract() const { return false; }

// Register an abstract entity - it cannot be created via the default engine call and instead is used as a basis for other entities
#define CbAbstractEntity(cpp_class, classname)                                                                                  \
    const char* cpp_class::Class() const { return classname; }                                                                  \
    bool cpp_class::IsAbstract() const { return true; }

namespace cbpp {
    class CBaseEntity;

    class CEntityRegistrator {
        public:
            typedef CBaseEntity* (*entfact_t)();
            CEntityRegistrator(const char* sClassName, entfact_t fpFactory);

            static entfact_t GetFactoryPointer(const char* sClassName);
    };

    typedef CBinTable<CConstString, CEntityRegistrator::entfact_t> entmap_t;

    entmap_t& GetEntityFactories();
}

#endif
