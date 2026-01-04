#include "cbpp/cml/cml.h"

#include "cbpp/string_utils.h"

namespace cbpp::cml {
    #define KW(_str) if( strcmp(sData, _str) == 0 ) { return true; }
    bool IsKeyword(const char* sData) {
        KW("include")
        KW("define")

        return false;
    }

    CValue::CValue(int32_t iValue) {
        m_iType = EValueType::Integer;
        m_Value.i32 = iValue;
    }

    CValue::CValue(float fValue) {
        m_iType = EValueType::Float;
        m_Value.f32 = fValue;
    }
    
    CValue::CValue(const char* sValue) {
        m_iType = EValueType::String;
        m_Value.str = StringDup(sValue);
    }

    CValue::CValue(const char* pValue, size_t iLength) {
        m_iType = EValueType::Binary;
        m_iLength = iLength;

        m_Value.str = Malloc<char>(iLength);
        memcpy(m_Value.str, pValue, iLength);
    }

    CValue::~CValue() {
        if((m_iType == EValueType::Binary || m_iType == EValueType::String) && m_Value.str != NULL) {
            Free(m_Value.str);
            m_Value.str = NULL;
        }
    }

    size_t CValue::GetLength() const {
        return m_iLength;
    }

    void CValue::SetValue(int32_t iValue) {
        if(m_iType == EValueType::Integer) {
            m_Value.i32 = iValue;
        }
    }

    void CValue::SetValue(float fValue) {
        if(m_iType == EValueType::Float) {
            m_Value.f32 = fValue;
        }
    }

    void CValue::SetValue(const char* sValue) {
        if(m_iType == EValueType::String) {
            if(m_Value.str != NULL) {
                Free(m_Value.str);
            }
            m_Value.str = StringDup(sValue);
        }
    }

    void CValue::SetValue(const char* pValue, size_t iLength) {
        if(m_iType == EValueType::Binary) {
            m_Value.str = Realloc<char>(m_Value.str, iLength);
            memcpy(m_Value.str, pValue, iLength);
            m_iLength = iLength;
        }
    }

    int32_t CValue::GetInt() const {
        if(m_iType == EValueType::Integer) {
            return m_Value.i32;
        } else {
            return 0;
        }
    }

    float CValue::GetFloat() const {
        if(m_iType == EValueType::Float) {
            return m_Value.f32;
        } else {
            return 0.0f;
        }
    }

    const char* CValue::GetString() const {
        if(m_iType == EValueType::String) {
            if(m_Value.str == NULL) { return "(null)"; }
            return const_cast<const char*>(m_Value.str);
        } else {
            return "(null)";
        }
    }

    const char* CValue::GetBinary() const {
        if(m_iType == EValueType::Binary) {
            return const_cast<const char*>(m_Value.str);
        } else {
            return NULL;
        }
    }
    
    void CValue::SetType(EValueType iType) {
        bool bSwitchString = iType != EValueType::String && m_iType == EValueType::String;
        bool bSwitchBinary = iType != EValueType::Binary && m_iType == EValueType::Binary;
        
        if(bSwitchBinary || bSwitchString) {
            Free(m_Value.str); // Switching from string or binary storage, have to free the memory
        }

        m_iType = iType;

        memset(&m_Value, 0, sizeof(m_Value));
    }

    EValueType CValue::Type() const {
        return m_iType;
    }
}
