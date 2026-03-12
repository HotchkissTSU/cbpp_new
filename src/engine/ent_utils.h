#ifndef CBPP_ENTITY_PROPERTY_H
#define CBPP_ENTITY_PROPERTY_H

/*
    Macros for entity creation
*/

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
            typedef ent::CBase* (*factory_t)();
            typedef IEntityDatadesc* (*datadesc_factory_t)(void);
            typedef void (*initfunc_t)(ent::CBase*, void*);

            CEntityRegistrator(const char* sClassname, factory_t fpConstr, datadesc_factory_t fpDatadescConstr, initfunc_t fpInit);
    };

    struct EntityRegistryInfo {
        CEntityRegistrator::factory_t fpEntityCreator;
        CEntityRegistrator::datadesc_factory_t fpDatadescCreator;
        CEntityRegistrator::initfunc_t fpInitFunc;
    };

    CBinTable<CConstString, EntityRegistryInfo>& GetEntFactoryMap();

    void InitEntity(ent::CBase* pEnt, const char* sClassname, void* pDatadesc);

    /*
        I AM THE LIVING GOD OF C++
        Can your filthy peasant Rust do this???
    */

    // Register this class as an entity
    #define CbEntity(_class, _base, ...)                                                                                            \
        public: virtual const char* Classname() { return #_class; }                                                                 \
        virtual const char* Base() { return #_base; }                                                                               \
        class Datadesc : public _base::Datadesc { public: virtual ~Datadesc() = default; __VA_ARGS__};                              \
        _class() : _base() { this->Construct(); }                                                                                   \
        virtual ~_class() { this->Destruct(); }                                                                                     \
        private: static IEntityDatadesc* CreateDatadesc() { return (cbpp::IEntityDatadesc*)(cbpp::New<Datadesc>()); }               \
        static cbpp::ent::CBase* CreateInstance() {                                                                                 \
            return (cbpp::ent::CBase*)(cbpp::New<_class>());}                                                                       \
        static void ConstructInstance(cbpp::ent::CBase* pEnt, void* pData) { cbpp::InitEntity(pEnt, #_base, pData);                 \
        (((_class*)pEnt)->Init((_class::Datadesc*)pData));}                                                                         \
        inline static cbpp::CEntityRegistrator __s_registrator =                                                                    \
            cbpp::CEntityRegistrator( #_class, _class::CreateInstance, _class::CreateDatadesc, _class::ConstructInstance );        

    /*
        An integer property with default settings
    */
    #define CbIntProperty(_member)                                                                                                  \
        public: int32_t _member; private:                                                                                           \
        CEntityPropsConstructor __cb_property_##_member = {__get_this(),                                                            \
        {((cbpp::IEntityProperty*)cbpp::New<cbpp::CNumberEntityProperty>(#_member, &_member))}};

    /*
        An integer property. Arguments are:
        NAME, DEFAULT_VALUE, MIN_VALUE, MAX_VALUE
    */
    #define CbIntPropertyEx(_member, default_value, min_value, max_value)                                                           \
        public: int32_t _member; private:                                                                                           \
        CEntityPropsConstructor __cb_property_##_member = {__get_this(),                                                            \
        {((cbpp::IEntityProperty*)cbpp::New<cbpp::CNumberEntityProperty>(#_member, &_member, (default_value), (min_value), (max_value)))}};

    /*
        A float property with default settings
    */
    #define CbFloatProperty(_member)                                                                                                \
        public: float _member; private:                                                                                             \
        CEntityPropsConstructor __cb_property_##_member = {__get_this(),                                                            \
        {((cbpp::IEntityProperty*)cbpp::New<cbpp::CFloatEntityProperty>(#_member, &_member))}};

    /*
        A float property. Arguments are:
        NAME, DEFAULT_VALUE, MIN_VALUE, MAX_VALUE
    */
    #define CbFloatPropertyEx(_member, default_value, min_value, max_value)                                                         \
        public: float _member; private:                                                                                             \
        CEntityPropsConstructor __cb_property_##_member = {__get_this(),                                                            \
        {((cbpp::IEntityProperty*)cbpp::New<cbpp::CFloatEntityProperty>(#_member, &_member, (default_value), (min_value), (max_value)))}};

    /*
        A string property with default settings
    */
    #define CbStringProperty(_member)                                                                                               \
        public: cbpp::CString _member; private:                                                                                     \
        CEntityPropsConstructor __cb_property_##_member = {__get_this(),                                                            \
        {((cbpp::IEntityProperty*)cbpp::New<cbpp::CStringEntityProperty>(#_member, &_member))}};

    /*
        A string property. Arguments are:
        NAME, DEFAULT_VALUE, STRING_TYPE
    */
    #define CbStringPropertyEx(_member, default_value, str_type)                                                                    \
        public: cbpp::CString _member; private:                                                                                     \
        CEntityPropsConstructor __cb_property_##_member = {__get_this(),                                                            \
        {((cbpp::IEntityProperty*)cbpp::New<cbpp::CStringEntityProperty>(#_member, &_member, (default_value), (str_type)))}};

    /*
        Enumeration property. Usage:
            CbEnumProperty(NAME, STR1, NUM1, STR2, NUM2 ...)

            Pass any amount of "string-number" pairs to the macro to add them to the enumeration
    */
    #define CbEnumProperty(_member, ...)                                                                                            \
        public: int32_t _member; private:                                                                                           \
        CEntityPropsConstructor __cb_property_##_member = {__get_this(),                                                            \
        {((cbpp::IEntityProperty*)cbpp::New<cbpp::CEnumEntityProperty>(#_member, (int32_t*)(&_member), 0, __VA_ARGS__))}};

    /*
        Enumeration property. Usage:
            CbEnumProperty(NAME, DEFAULT_VALUE, STR1, NUM1, STR2, NUM2 ...)

            Pass any amount of "string-number" pairs to the macro to add them to the enumeration
    */
    #define CbEnumPropertyEx(_member, default_value, ...)                                                                           \
        public: int32_t _member; private:                                                                                           \
        CEntityPropsConstructor __cb_property_##_member = {__get_this(),                                                            \
        {((cbpp::IEntityProperty*)cbpp::New<cbpp::CEnumEntityProperty>(#_member, (int32_t*)(&_member), (default_value), __VA_ARGS__))}};

    /*
        A vector property with default settings
    */
    #define CbVectorProperty(_member)                                                                                               \
        public: cbpp::Vec2f _member; private:                                                                                       \
        CEntityPropsConstructor __cb_property_##_member = {__get_this(),                                                            \
        {((cbpp::IEntityProperty*)cbpp::New<cbpp::CVectorEntityProperty>(#_member, &_member))}};

    /*
        A vector property. Arguments are:
        NAME, DEFAULT_VALUE, MIN_BOUND (vector), MAX_BOUND (vector)
    */
    #define CbVectorPropertyEx(_member, def_x, def_y, min_x, min_y, max_x, max_y)                                                   \
        public: cbpp::Vec2f _member; private:                                                                                       \
        CEntityPropsConstructor __cb_property_##_member = {__get_this(),                                                            \
        {((cbpp::IEntityProperty*)cbpp::New<cbpp::CVectorEntityProperty>(#_member, &_member, cbpp::Vec2f(def_x, def_y),             \
            cbpp::Vec2f(min_x, min_y), cbpp::Vec2f(max_x, max_y)))}};

    /*
        A color property with default settings
    */
    #define CbColorProperty(_member)                                                                                                \
        public: cbpp::Color _member; private:                                                                                       \
        CEntityPropsConstructor __cb_property_##_member = {__get_this(),                                                            \
        {((cbpp::IEntityProperty*)cbpp::New<cbpp::CColorEntityProperty>(#_member, &_member))}};

    /*
        A color property. Arguments are:
        NAME, DEFAULT_R, DEFAULT_G, DEFAULT_B, DEFAULT_A
    */
    #define CbColorPropertyEx(_member, def_r, def_g, def_b, def_a)                                                                  \
        public: cbpp::Color _member; private:                                                                                       \
        CEntityPropsConstructor __cb_property_##_member = {__get_this(),                                                            \
        {((cbpp::IEntityProperty*)cbpp::New<cbpp::CColorEntityProperty>(#_member, &_member,                                         \
            cbpp::Color((uint8_t)(def_r), (uint8_t)(def_g), (uint8_t)(def_b), (uint8_t)(def_a))))}};
}

#endif
