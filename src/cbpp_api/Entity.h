
/*
    CB++ engine entity API

    Here are some rules about CB++ networking design:

    1) Server is always true

    2) Clientside entities shall only perform rendering, input handling and some other non-important code,
        unless the entity is completely clientside.

    3) Vice versa, serverside entities do all the work: listen for client`s inputs, do thingies and then send themselves to
        clients for them to copy and interp to.

    4) Entity IDs must be synced between all clients, if the entity is not completely clientside. Otherwise, such entities must not collide
        with other server-managed IDs. Server does all of the entity creation/deletion stuff and then tells everybody via the net messages

    5) Clients only receive updates on entities inside their PVS. Getting in/out of the PVS should not be interpolated. 

    6) Clientside / serverside class interfaces should be as similar as possible in the sake of readability. Functions, which are restricted
        for a specific side, should throw static assertions.
*/

#ifndef CBPP_ENTITY_API_H
#define CBPP_ENTITY_API_H

#include <stddef.h>
#include <stdint.h>

#include "cbpp/malloc_wrapper.h"

#define CBPP_DEFAULT_ENTITY_DESC "Default description"

/*
    Begin describing this classes dynamic properties

    Usage:

    CbProperties { CbProperty(...), CbProperty(...), ... }
*/
#define CbProperties virtual const char* Class() const; virtual void ConstructProps()

/*
    Declare that this entity class has no own properties
    
    Expands to CbProperties() {}
*/
#define CbNoProperties virtual const char* Class() const; virtual void ConstructProps() {}

/*
    Declare a property

    val - name of the class`es field to describe
    name - internal property name
    pname - "pretty" name, accepts locale keys
    type - general type, or EGenericType::Incompat if something uncommon 
*/

#define CbProperty(val, name, pname, type) cbpp::New<cbpp::CProperty<decltype(val)>>(&val, this, name, pname, CBPP_DEFAULT_ENTITY_DESC, type, 1)

/*
    Declare an array property

    Array properties have constant size and are used to describe simple structures like color(3 integers) or vector(2 floats) etc.

    val - name of the class`es field to describe
    name - internal property name
    pname - "pretty" name, accepts locale keys
    desc - description, accepts locale keys
    type - general type, or GType_Incompat if something uncommon 
*/
#define CbArrayPropertyEx(val, name, pname, desc, type, length) cbpp::New<cbpp::CProperty<decltype(val)>>(&val, this, name, pname, desc, type, length)

/*
    Declare an array property with a description
    Array properties have constant size and are used to describe simple structures like color(3 integers) or vector(2 floats) etc.

    val - name of the class`es field to describe
    name - internal property name
    pname - "pretty" name or locale key
    type - general type, or GType_Incompat if something uncommon 
*/
#define CbArrayProperty(val, name, pname, type, length) cbpp::New<cbpp::CProperty<decltype(val)>>(&val, this, name, pname, CBPP_DEFAULT_ENTITY_DESC, type, length)

/*
    Declare a property with a description

    val - name of the class`es field to describe
    name - internal property name
    pname - "pretty" name, accepts locale keys
    desc - description, accepts locale keys
    type - general type, or GType_Incompat if something uncommon 
*/
#define CbPropertyEx(val, name, pname, desc, type) new cbpp::CProperty<decltype(val)>(&val, this, name, pname, desc, type, 1)

// Register an entity to allow its creation via the text label
#define CbRegisterEntity(cpp_class, classname)                                                                                  \
    cbpp::CBaseEntity* __entfact_##cpp_class() { cpp_class* eNew = cbpp::New<cpp_class>(); return eNew; }                       \
    static cbpp::CEntityRegistrator g_hEntityReg_##cpp_class(classname, __entfact_##cpp_class);                                 \
    const char* cpp_class::Class() const { return classname; }

// Register an abstract entity - it cannot be created via the default engine call and instead is used as a basis for other entities
#define CbAbstractEntity(cpp_class, classname) const char* cpp_class::Class() const { return classname; }

namespace cbpp {
    typedef uint32_t eid_t;

    /*
        Generic type marks for UI input
    */
    enum class EGenericType : uint32_t {
        Incompat,
        Integer32,
        Integer64,
        Float,
        Vector2D,
        Vector3D,
        Color,
        String
    };

    class CBaseEntity;

    class IProperty {
        protected:
            IProperty* m_pNext = NULL;
        public:
            IProperty* Next();
            void SetNextNode(IProperty* pNext);

            virtual void* GetBuffer() = 0;
            virtual size_t Sizeof() = 0;
            virtual EGenericType Type() = 0;

            virtual void* Index(size_t iIndex) = 0;
            virtual size_t Length() = 0;

            virtual const char* Name() = 0;
            virtual const char* PrettyName() = 0;
            virtual const char* Description() = 0;

            // Get the owner of this property
            virtual CBaseEntity* Master() = 0;

            virtual ~IProperty() = default;
    };

    class CBaseEntity {
        eid_t m_iUID;
        IProperty* m_pFirstProp = NULL;

        protected:
            void SetUID(eid_t iNewID);

        public:
            CbNoProperties;

            virtual void Think() = 0;
            virtual void Render() = 0;
            virtual void NetSync() = 0;

            void PushProperty(IProperty* pProp);
            IProperty* GetProperties();
            const IProperty* GetProperties() const;
            IProperty* GetPropertyByName(const char* sName);

            eid_t UID() const;

            CBaseEntity() {};
            virtual ~CBaseEntity();
    };

    /*
        Create an entity of this classname
    */
    CBaseEntity* CreateEntityClass(const char* sClassName);
}

#endif
