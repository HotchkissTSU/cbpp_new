#include "cbpp/CML.h"

#include <math.h>

#include "engine/cml/object.h"

// CObject

namespace cbpp::cml {
    const CObject NIL = {NULL};

    CObject::CObject(IObject* pData) : m_pObj(pData) {}
    CObject::operator IObject*() const { return m_pObj; }
    IObject* CObject::GetPointer() const { return m_pObj; }

    bool CObject::operator==(const CObject& pOther) const {
        return m_pObj == pOther.m_pObj;
    }

    bool CObject::operator!=(const CObject& pOther) const {
        return m_pObj != pOther.m_pObj;
    }

    EObjectClass CObject::Class() const { return m_pObj->Class(); }
    size_t CObject::Length() const { return m_pObj->Length(); }

    CObject& CObject::operator=(int_t iValue) { m_pObj->SetValue(iValue); return *this; }
    CObject& CObject::operator=(float_t fValue) { m_pObj->SetValue(fValue); return *this; }
    CObject& CObject::operator=(const char* sValue) { m_pObj->SetValue(sValue); return *this; }
    void CObject::SetBinaryData(const uint8_t* pData, size_t iLength) { m_pObj->SetValue(pData, iLength); }

    CObject CObject::operator[](size_t iIndex) { return CObject( m_pObj->At(iIndex) ); }
    CObject CObject::operator[](const char* sName) { return CObject( m_pObj->At(sName) ); }

    void CObject::Push(CObject pObj) { m_pObj->PushValue(pObj.GetPointer()); }
    void CObject::Push(const char* sName, CObject pObj) { m_pObj->PushValue(sName, pObj.GetPointer()); }

    CObject::operator int_t() const { return m_pObj->AsInt(); }
    CObject::operator float_t() const { return m_pObj->AsFloat(); }
    CObject::operator const char*() const { return m_pObj->AsString(); }
    CObject::operator uint8_t*() { return m_pObj->AsBinary(); }
}

// CIntObject

namespace cbpp::cml {
    int_t CIntObject::AsInt() const {
        return m_iData;
    }

    float_t CIntObject::AsFloat() const {
        return (float_t)(m_iData);
    }

    const char* CIntObject::AsString() const {
        return NULL;
    }

    uint8_t* CIntObject::AsBinary() {
        return NULL;
    }

    void CIntObject::SetValue(int_t iData) {
        m_iData = iData;
    }

    void CIntObject::SetValue(float_t fData) {
        m_iData = (int_t)(roundf(fData));
    }

    void CIntObject::SetValue(const char* sData) {
        m_iData = 0;
    }

    void CIntObject::SetValue(const uint8_t* pData, size_t iLength) {
        m_iData = 0;
    }

    IObject* CIntObject::At(const char*) {
        return NULL;
    }

    IObject* CIntObject::At(size_t) {
        return NULL;
    }

    void CIntObject::PushValue(IObject* pObj) {}
    void CIntObject::PushValue(const char* sName, IObject* pObj) {}

    EObjectClass CIntObject::Class() const {
        return EObjectClass::Integer;
    }

    size_t CIntObject::Length() const {
        return 0;
    }
}

// CBinaryObject

namespace cbpp::cml {
    int_t CBinaryObject::AsInt() const {
        return 0;
    }

    float_t CBinaryObject::AsFloat() const {
        return 0.0f;
    }

    const char* CBinaryObject::AsString() const {
        return "<binary>";
    }

    uint8_t* CBinaryObject::AsBinary() {
        return m_aData.Data();
    }

    void CBinaryObject::SetValue(int_t iData) {}

    void CBinaryObject::SetValue(float_t fData) {}

    void CBinaryObject::SetValue(const char* sData) {}

    void CBinaryObject::SetValue(const uint8_t* pData, size_t iLength) {
        m_aData.SetArray(pData, iLength);
    }

    IObject* CBinaryObject::At(const char*) {
        return NULL;
    }

    IObject* CBinaryObject::At(size_t) {
        return NULL;
    }

    void CBinaryObject::PushValue(IObject* pObj) {}
    void CBinaryObject::PushValue(const char* sName, IObject* pObj) {}

    EObjectClass CBinaryObject::Class() const {
        return EObjectClass::Binary;
    }

    size_t CBinaryObject::Length() const {
        return m_aData.Length();
    }
}

// CFloatObject

namespace cbpp::cml {
    int_t CFloatObject::AsInt() const {
        return (int_t)(m_fData);
    }

    float_t CFloatObject::AsFloat() const {
        return m_fData;
    }

    const char* CFloatObject::AsString() const {
        return NULL;
    }

    uint8_t* CFloatObject::AsBinary() {
        return NULL;
    }

    void CFloatObject::SetValue(int_t iData) {
        m_fData = (float_t)(iData);
    }

    void CFloatObject::SetValue(float_t fData) {
        m_fData = fData;
    }

    void CFloatObject::SetValue(const char* sData) {
        m_fData = 0.0f;
    }

    void CFloatObject::SetValue(const uint8_t* pData, size_t iLength) {
        m_fData = 0.0f;
    }

    IObject* CFloatObject::At(const char*) {
        return NULL;
    }

    IObject* CFloatObject::At(size_t) {
        return NULL;
    }

    void CFloatObject::PushValue(IObject* pObj) {}
    void CFloatObject::PushValue(const char* sName, IObject* pObj) {}

    EObjectClass CFloatObject::Class() const {
        return EObjectClass::Float;
    }

    size_t CFloatObject::Length() const {
        return 0;
    }
}

// CStringObject

namespace cbpp::cml {
    int_t CStringObject::AsInt() const {
        return 0;
    }

    float_t CStringObject::AsFloat() const {
        return 0.0f;
    }

    const char* CStringObject::AsString() const {
        return m_sData.String();
    }

    uint8_t* CStringObject::AsBinary() {
        return (uint8_t*)(m_sData.Pointer());
    }

    void CStringObject::SetValue(int_t iData) {}

    void CStringObject::SetValue(float_t fData) {}

    void CStringObject::SetValue(const char* sData) {
        m_sData.Set(sData);
    }

    void CStringObject::SetValue(const uint8_t* pData, size_t iLength) {}

    IObject* CStringObject::At(const char*) {
        return NULL;
    }

    IObject* CStringObject::At(size_t) {
        return NULL;
    }

    void CStringObject::PushValue(IObject* pObj) {}
    void CStringObject::PushValue(const char* sName, IObject* pObj) {}

    EObjectClass CStringObject::Class() const {
        return EObjectClass::String;
    }

    size_t CStringObject::Length() const {
        return m_sData.Length();
    }
}

// CArrayObject

namespace cbpp::cml {
    int_t CArrayObject::AsInt() const {
        return 0;
    }

    float_t CArrayObject::AsFloat() const {
        return 0.0f;
    }

    const char* CArrayObject::AsString() const {
        return "<array>";
    }

    uint8_t* CArrayObject::AsBinary() {
        return NULL;
    }

    void CArrayObject::SetValue(int_t iData) {}

    void CArrayObject::SetValue(float_t fData) {}

    void CArrayObject::SetValue(const char* sData) {}

    void CArrayObject::SetValue(const uint8_t* pData, size_t iLength) {}

    IObject* CArrayObject::At(const char*) {
        return NULL;
    }

    IObject* CArrayObject::At(size_t iIndex) {
        if(iIndex >= m_pData.Length()) { return NULL; }
        return m_pData[iIndex];
    }

    void CArrayObject::PushValue(IObject* pObj) {
        m_pData.PushBack(pObj);
    }

    void CArrayObject::PushValue(const char* sName, IObject* pObj) {
        m_pData.PushBack(pObj);
    }

    EObjectClass CArrayObject::Class() const {
        return EObjectClass::Array;
    }

    size_t CArrayObject::Length() const {
        return m_pData.Length();
    }

    CArrayObject::~CArrayObject() {
        for(size_t i = 0; i < m_pData.Length(); i++) {
            DeleteObject(m_pData[i]);
        }
    }
}

// CDictObject

namespace cbpp::cml {
    int_t CDictObject::AsInt() const {
        return 0;
    }

    float_t CDictObject::AsFloat() const {
        return 0.0f;
    }

    const char* CDictObject::AsString() const {
        return "<table>";
    }

    uint8_t* CDictObject::AsBinary() {
        return NULL;
    }

    void CDictObject::SetValue(int_t iData) {}

    void CDictObject::SetValue(float_t fData) {}

    void CDictObject::SetValue(const char* sData) {}

    void CDictObject::SetValue(const uint8_t* pData, size_t iLength) {}

    IObject* CDictObject::At(const char* sName) {
        IObject** pObj = m_dTable.At(sName);
        if(pObj == NULL) { return NULL; }
        return *pObj;
    }

    IObject* CDictObject::At(size_t iIndex) {
        if(iIndex >= m_dTable.Length()) { return NULL; }
        return m_dTable.Index(iIndex);
    }

    void CDictObject::PushValue(IObject* pObj) { }

    void CDictObject::PushValue(const char* sName, IObject* pObj) {
        m_dTable.Insert(sName, pObj);
    }

    EObjectClass CDictObject::Class() const {
        return EObjectClass::Object;
    }

    size_t CDictObject::Length() const {
        return m_dTable.Length();
    }

    CDictObject::~CDictObject() {
        for(size_t i = 0; i < m_dTable.Length(); i++) {
            DeleteObject(m_dTable.Index(i));
        }
    }
}

namespace cbpp::cml {
    CObject CreateObject(EObjectClass iClass) {
        IObject* pObj = NULL;

        switch(iClass) {
            case EObjectClass::Array:
                pObj = cbpp::New<CArrayObject>(); break;

            case EObjectClass::Binary:
                pObj = cbpp::New<CBinaryObject>(); break;

            case EObjectClass::Float:
                pObj = cbpp::New<CFloatObject>(); break;

            case EObjectClass::Integer:
                pObj = cbpp::New<CIntObject>(); break;

            case EObjectClass::Object:
                pObj = cbpp::New<CDictObject>(); break;

            case EObjectClass::String:
                pObj = cbpp::New<CStringObject>(); break;

            default:
                CbAssertf(true, "Unknown object type (%i), update le enum", iClass);
        }

        return CObject(pObj);
    }

    void DeleteObject(CObject pObj) {
        if( pObj.GetPointer() == NULL ) {
            return;
        }

        cbpp::Delete(pObj.GetPointer());
    }

    void PrintTabs(size_t iNum) {
        for(size_t i = 0; i < iNum; i++) {
            printf("  ");
        }
    }

    void PrintObject(CObject pObj, size_t iDepth) {
        PrintTabs(iDepth);
        switch(pObj.Class()) {
            case EObjectClass::Array: {
                for(size_t i = 0; i < pObj.Length(); i++) {
                    PrintObject( pObj[i], iDepth+1 );
                }
                break;
            }

            case EObjectClass::Object: {
                for(size_t i = 0; i < pObj.Length(); i++) {
                    PrintObject( pObj[i], iDepth+1 );
                }
                break;
            }

            case EObjectClass::Integer: {
                printf("%d\n", (int_t)(pObj));
                break;
            }

            case EObjectClass::String: {
                printf("%s\n", (const char*)(pObj));
                break;
            }
        }
    }
}
