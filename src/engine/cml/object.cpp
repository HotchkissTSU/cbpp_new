#include "cbpp/CML.h"

#include <math.h>

#include "engine/cml/object.h"

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

    uint8_t* CBinaryObject::AsBinary() {
        return m_aData.Data();
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

    IObject* CIntObject::At(const char*) {
        return NULL;
    }

    IObject* CIntObject::At(size_t) {
        return NULL;
    }

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
        return NULL;
    }

    void CBinaryObject::SetValue(int_t iData) {}

    void CBinaryObject::SetValue(float_t fData) {}

    void CBinaryObject::SetValue(const char* sData) {}

    IObject* CBinaryObject::At(const char*) {
        return NULL;
    }

    IObject* CBinaryObject::At(size_t) {
        return NULL;
    }

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

    void CFloatObject::SetValue(int_t iData) {
        m_fData = (float_t)(iData);
    }

    void CFloatObject::SetValue(float_t fData) {
        m_fData = fData;
    }

    void CFloatObject::SetValue(const char* sData) {
        m_fData = 0;
    }

    IObject* CFloatObject::At(const char*) {
        return NULL;
    }

    IObject* CFloatObject::At(size_t) {
        return NULL;
    }

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

    void CStringObject::SetValue(int_t iData) {}

    void CStringObject::SetValue(float_t fData) {}

    void CStringObject::SetValue(const char* sData) {
        m_sData.Set(sData);
    }

    IObject* CStringObject::At(const char*) {
        return NULL;
    }

    IObject* CStringObject::At(size_t) {
        return NULL;
    }

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

    void CArrayObject::SetValue(int_t iData) {}

    void CArrayObject::SetValue(float_t fData) {}

    void CArrayObject::SetValue(const char* sData) {}

    IObject* CArrayObject::At(const char*) {
        return NULL;
    }

    IObject* CArrayObject::At(size_t iIndex) {
        IObject** pObj = m_pData.At(iIndex);
        if(pObj == NULL) { return NULL; }
        return *pObj;
    }

    EObjectClass CArrayObject::Class() const {
        return EObjectClass::Array;
    }

    size_t CArrayObject::Length() const {
        return m_pData.Length();
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

    void CDictObject::SetValue(int_t iData) {}

    void CDictObject::SetValue(float_t fData) {}

    void CDictObject::SetValue(const char* sData) {}

    IObject* CDictObject::At(const char* sName) {
        IObject** pObj = m_dTable.At(sName);
        if(pObj == NULL) { return NULL; }
        return *pObj;
    }

    IObject* CDictObject::At(size_t iIndex) {
        return m_dTable.Index(iIndex);
    }

    EObjectClass CDictObject::Class() const {
        return EObjectClass::Object;
    }

    size_t CDictObject::Length() const {
        return m_dTable.Length();
    }
}
