#ifndef CBPP_ENTITY_PROPERTY_H
#define CBPP_ENTITY_PROPERTY_H

#include <initializer_list>

#include "cbpp/Table.h"
#include "cbpp/String.h"
#include "cbpp/Memory.h"

#include "engine/ent_property.h"

namespace cbpp {
    namespace ent { class CBase; }  
    
    class IEntityDatadesc {
        public:
            virtual size_t Length() = 0;
            virtual IEntityProperty* At(size_t iIndex) = 0;

            virtual ~IEntityDatadesc() = default;
    };

    class CEntityPropsConstructor {
        public:
            CEntityPropsConstructor(void* pParent, std::initializer_list<IEntityProperty*> aProps);
    };

    class CEntityRegistrator {
        public:
            typedef ent::CBase* (*factory_t)(void*);
            typedef IEntityDatadesc* (*datadesc_factory_t)(void);

            CEntityRegistrator(const char* sClassname, factory_t fpConstr, datadesc_factory_t fpDatadescConstr);
    };

    struct EntityRegistryInfo {
        CEntityRegistrator::factory_t fpEntityCreator;
        CEntityRegistrator::datadesc_factory_t fpDatadescCreator;
    };

    CBinTable<CConstString, EntityRegistryInfo>& GetEntFactoryMap();

    #define CbEntity(_class, _base, ...)                                                                                            \
        public: virtual const char* Classname() { return #_class; }                                                                 \
        virtual const char* Base() { return #_base; }                                                                               \
        class Datadesc : public _base::Datadesc { public: virtual ~Datadesc() = default; __VA_ARGS__;};                             \
        _class(Datadesc* pData) : _base(pData) { this->Init(pData); }                                                               \
        ~_class() { this->Destruct(); }                                                                                             \
        private: static IEntityDatadesc* CreateDatadesc() { return (cbpp::IEntityDatadesc*)(cbpp::New<Datadesc>()); }               \
        static cbpp::ent::CBase* CreateInstance(void* pData) {                                                                      \
            return (cbpp::ent::CBase*)(cbpp::New<_class>((Datadesc*)pData));}                                                       \
        inline static cbpp::CEntityRegistrator __s_registrator =                                                                    \
            cbpp::CEntityRegistrator( #_class, _class::CreateInstance, _class::CreateDatadesc );   
            
    #define CbIntProperty(_member)                                                                                                  \
        public: int32_t _member; private:                                                                                           \
        CEntityPropsConstructor __cb_property_##_member = {__get_this(),                                                            \
        {((cbpp::IEntityProperty*)cbpp::New<cbpp::CNumberEntityProperty>(#_member, &_member))}};

    #define CbIntPropertyEx(_member, ...)                                                                                           \
        public: int32_t _member; private:                                                                                           \
        CEntityPropsConstructor __cb_property_##_member = {__get_this(),                                                            \
        {((cbpp::IEntityProperty*)cbpp::New<cbpp::CNumberEntityProperty>(#_member, &_member, __VA_ARGS__))}};

    #define CbFloatProperty(_member)                                                                                                \
        public: float _member; private:                                                                                             \
        CEntityPropsConstructor __cb_property_##_member = {__get_this(),                                                            \
        {((cbpp::IEntityProperty*)cbpp::New<cbpp::CFloatEntityProperty>(#_member, &_member))}};

    #define CbFloatPropertyEx(_member, ...)                                                                                         \
        public: float _member; private:                                                                                             \
        CEntityPropsConstructor __cb_property_##_member = {__get_this(),                                                            \
        {((cbpp::IEntityProperty*)cbpp::New<cbpp::CFloatEntityProperty>(#_member, &_member, __VA_ARGS__))}};
}

#endif
