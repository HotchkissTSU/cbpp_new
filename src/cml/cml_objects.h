#ifndef CBPP_CML_OBJECTS_H
#define CBPP_CML_OBJECTS_H

#include "cbpp/CML.h"

namespace cbpp::cml {
    class CValueObject final : public IObject {
        friend IObject* CreateObject(EValueType);

        CValue m_Value;

        public:
            CValueObject(EValueType iType);

            EValueType Type() const override;
            CValue* Value() override;
            
            objlist_t* GetArray() override;
            objmap_t* GetChildren() override;

            bool PushChild(const char* sName, IObject* pChild) override;

            bool HasChild(const char* sName) const override;
            size_t Length() const override;

            IObject* GetByName(const char* sName) const override;
            IObject* GetByIndex(size_t iIndex) const override;

            IObject* GetCopy() override;

            virtual ~CValueObject() override = default;
    };

    class CObject final : public IObject {
        friend IObject* CreateObject(EValueType);

        objmap_t m_dChildren;

        public:
            EValueType Type() const override;
            CValue* Value() override;
            
            objlist_t* GetArray() override;
            objmap_t* GetChildren() override;

            bool PushChild(const char* sName, IObject* pChild) override;

            bool HasChild(const char* sName) const override;
            size_t Length() const override;

            IObject* GetByName(const char* sName) const override;
            IObject* GetByIndex(size_t iIndex) const override;

            IObject* GetCopy() override;

            virtual ~CObject() override;
    };

    class CArrayObject final : public IObject {
        friend IObject* CreateObject(EValueType);

        CArray<IObject*> m_aChildren;

        public:
            EValueType Type() const override;
            CValue* Value() override;
            
            objmap_t* GetChildren() override;
            objlist_t* GetArray() override;

            bool PushChild(const char* sName, IObject* pChild) override;

            bool HasChild(const char* sName) const override;
            size_t Length() const override;

            IObject* GetByName(const char* sName) const override;
            IObject* GetByIndex(size_t iIndex) const override;

            IObject* GetCopy() override;

            virtual ~CArrayObject() override;
    };
}

#endif
