#include "cbpp/cml/cml_objects.h"

namespace cbpp::cml {
    // ++ CValueObject ++

    CValueObject::CValueObject(EValueType iType) {
        m_Value.SetType(iType);
    }

    EValueType CValueObject::Type() const {
        return m_Value.Type();
    }

    CValue* CValueObject::Value() {
        return &m_Value;
    }

    IObject::objmap_t* CValueObject::GetChildren() {
        return NULL;
    }

    IObject::objlist_t* CValueObject::GetArray() {
        return NULL;
    }

    bool CValueObject::PushChild(const char* sName, IObject* pChild) {
        return false;
    }

    IObject* CValueObject::GetByName(const char* sName) const {
        return NULL;
    }

    IObject* CValueObject::GetByIndex(size_t iIndex) const {
        return NULL;
    }

    bool CValueObject::HasChild(const char* sName) const {
        return false;
    }

    size_t CValueObject::Length() const {
        return 1;
    }

    // ++ CObject ++

    EValueType CObject::Type() const {
        return EValueType::Object;
    }

    CValue* CObject::Value() {
        return NULL;
    }

    IObject::objmap_t* CObject::GetChildren() {
        return &m_dChildren;
    }

    IObject::objlist_t* CObject::GetArray() {
        return NULL;
    }

    bool CObject::PushChild(const char* sName, IObject* pChild) {
        if(pChild == NULL) { return false; }
        m_dChildren[sName] = pChild;
        return true;
    }

    IObject* CObject::GetByName(const char* sName) const {
        IObject* const* pTest = m_dChildren.At(sName);
        if(pTest == NULL) {
            return NULL;
        } else {
            return *pTest;
        }
    }

    IObject* CObject::GetByIndex(size_t iIndex) const {
        return NULL;
    }

    bool CObject::HasChild(const char* sName) const {
        return m_dChildren.HasKey(sName);
    }

    size_t CObject::Length() const {
        return m_dChildren.Length();
    }

    CObject::~CObject() {
        const objmap_t::pairs_t& aPairs = m_dChildren.Data();
        for(size_t i = 0; i < aPairs.Length(); i++) {
            Delete(aPairs[i].Value);
        }
    }

    // ++ CArrayObject ++

    EValueType CArrayObject::Type() const {
        return EValueType::Array;
    }

    CValue* CArrayObject::Value() {
        return NULL;
    }

    IObject::objmap_t* CArrayObject::GetChildren() {
        return NULL;
    }

    IObject::objlist_t* CArrayObject::GetArray() {
        return &m_aChildren;
    }

    bool CArrayObject::PushChild(const char* sName, IObject* pChild) {
        if(pChild == NULL) { return false; }
        m_aChildren.PushBack(pChild);
        return true;
    }

    IObject* CArrayObject::GetByName(const char* sName) const {
        return NULL;
    }

    IObject* CArrayObject::GetByIndex(size_t iIndex) const {
        if(iIndex >= m_aChildren.Length()) {
            return NULL;
        }

        return m_aChildren[iIndex];
    }

    size_t CArrayObject::Length() const {
        return m_aChildren.Length();
    }

    bool CArrayObject::HasChild(const char* sName) const {
        return false;
    }

    CArrayObject::~CArrayObject() {
        for(size_t i = 0; i < m_aChildren.Length(); i++) {
            Delete(m_aChildren[i]);
        }
    }

    // ++ Allocation functions ++

    IObject* CreateObject(EValueType iType) {
        switch(iType) {
            case EValueType::Array:
                return static_cast<IObject*>( New<CArrayObject>() );

            case EValueType::Object:
                return static_cast<IObject*>( New<CObject>() );

            default:
                return static_cast<IObject*>( New<CValueObject>(iType) );
        }
    }

    void PutIndent(size_t iAmount) {
        for(size_t i = 0; i < iAmount; i++) {
            printf("\t");
        }
    }

    void PrintObject(IObject* pObj, size_t iDepth, const char* sName) {
        switch(pObj->Type()) {
            case EValueType::Integer:
                PutIndent(iDepth);
                printf("INT %s = %i\n", sName, pObj->Value()->GetInt());
                break;

            case EValueType::Float:
                PutIndent(iDepth);
                printf("FLT %s = %f\n", sName, pObj->Value()->GetFloat());
                break;

            case EValueType::String:
                PutIndent(iDepth);
                printf("STR %s = %s\n", sName, pObj->Value()->GetString());
                break;

            case EValueType::Object:
                IObject::objmap_t* dSubs = pObj->GetChildren();
                const IObject::objmap_t::pairs_t& aSubs = dSubs->Data();

                PutIndent(iDepth);
                printf("OBJ %s:\n", sName);
                for(size_t i = 0; i < aSubs.Length(); i++) {
                    PrintObject(aSubs[i].Value, iDepth+1, aSubs[i].Key.String());
                }

                break;
        }    
    }
}
