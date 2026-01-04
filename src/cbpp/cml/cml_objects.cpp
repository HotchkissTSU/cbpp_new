#include "cbpp/cml/cml_objects.h"

#include "cbpp/string_utils.h"

namespace cbpp::cml {
    // ++ CValueObject ++

    CValueObject::CValueObject(EValueType iType) {
        m_Value.SetType(iType);
    }

    IObject* CValueObject::GetCopy() {
        IObject* pCopy = CreateObject(m_Value.Type());

        if(m_Value.Type() == EValueType::String) {
            pCopy->Value()->m_Value.str = StringDup(m_Value.m_Value.str);

        } else if(m_Value.Type() == EValueType::Binary) {
            pCopy->Value()->m_Value.str = Malloc<char>(m_Value.GetLength());
            pCopy->Value()->m_iLength = m_Value.GetLength();
            memcpy(pCopy->Value()->m_Value.str, m_Value.GetBinary(), m_Value.GetLength());

        } else {
            memcpy(pCopy->Value(), &m_Value, sizeof(m_Value));
        }

        return pCopy;
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

    IObject* CObject::GetCopy() {
        IObject* pCopy = CreateObject(EValueType::Object);
        CObject* pObj = (CObject*)(pCopy);

        pObj->m_dChildren = m_dChildren;

        return pCopy;
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

    IObject* CArrayObject::GetCopy() {
        IObject* pCopy = CreateObject(EValueType::Array);
        CArrayObject* pArr = (CArrayObject*)(pCopy);

        pArr->m_aChildren = m_aChildren;

        return pCopy;
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

    // ++ Utility functions ++

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
                if(sName != NULL) {
                    PutIndent(iDepth);
                    printf("INT %s = %i\n", sName, pObj->Value()->GetInt());
                } else {
                    printf("INT = %i\n", pObj->Value()->GetInt());
                }
                break;

            case EValueType::Float:
                if(sName != NULL) {
                    PutIndent(iDepth);
                    printf("FLT %s = %f\n", sName, pObj->Value()->GetFloat());
                } else {
                    printf("FLT = %f\n", pObj->Value()->GetFloat());
                }
                break;

            case EValueType::String:
                if(sName != NULL) {
                    char sBuffer[CBPP_CML_MAX_LEXEM_LENGTH+1];

                    PutIndent(iDepth);
                    size_t iLn = snprintf(sBuffer, sizeof(sBuffer), "STR %s = ", sName);

                    printf(sBuffer);

                    char* sData = (char*)(pObj->Value()->GetString());
                    while(*sData != '\0') {
                        char cCurrent = *sData;
                        putc(cCurrent, stdout);
                        if(cCurrent == '\n') {
                            PutIndent(iDepth);
                            for(int i = 0; i < iLn; i++) { putc(' ', stdout); }
                        }

                        sData++;
                    }
                    putc('\n', stdout);
                } else {
                    printf("STR = %s\n", pObj->Value()->GetString());
                }
                break;

            case EValueType::Object: {
                IObject::objmap_t* dSubs = pObj->GetChildren();
                const IObject::objmap_t::pairs_t& aSubs = dSubs->Data();

                if(sName != NULL) {
                    PutIndent(iDepth);
                    printf("OBJ %s:\n", sName);
                } else {
                    printf("OBJ:\n");
                }

                for(size_t i = 0; i < aSubs.Length(); i++) {
                    PrintObject(aSubs[i].Value, iDepth+1, aSubs[i].Key.String());
                }

                break;
            }

            case EValueType::Array: {
                IObject::objlist_t* aArrSubs = pObj->GetArray();

                if(sName != NULL) {
                    PutIndent(iDepth);
                    printf("ARR %s:\n", sName);
                } else {
                    printf("ARR:\n");
                }
                
                for(size_t i = 0; i < aArrSubs->Length(); i++) {
                    PutIndent(iDepth+1);
                    printf("[%i] ", i);
                    PrintObject(aArrSubs->At(i), iDepth+1, NULL);
                }
                break;
            }
            
            case EValueType::Binary: {                
                if(sName != NULL) {
                    PutIndent(iDepth);
                    printf("BIN %s (%i)\n", sName, pObj->Value()->GetLength());
                } else {
                    printf("BIN (%i)\n", pObj->Value()->GetLength());
                }

                break;
            }
        }    
    }
}
