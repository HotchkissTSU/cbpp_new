#include "cbpp/cml/cml.h"

namespace cbpp::cml {
    #define KW(_str) if( strcmp(sData, _str) == 0 ) { return true; }
    bool IsKeyword(const char* sData) {
        KW("include")

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

    CValue::~CValue() {
        if(m_iType == EValueType::String) {
            Free(m_Value.str);
        }
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

    int32_t CValue::GetInt() const {
        if(m_iType == EValueType::Integer) {
            return m_Value.i32;
        }else{
            return 0;
        }
    }

    float CValue::GetFloat() const {
        if(m_iType == EValueType::Float) {
            return m_Value.f32;
        }else{
            return 0.0f;
        }
    }

    const char* CValue::GetString() const {
        if(m_iType == EValueType::String) {
            if(m_Value.str == NULL) { return "(null)"; }
            return const_cast<const char*>(m_Value.str);
        }else{
            return "(null)";
        }
    }

    void CValue::SetType(EValueType iType) {
        if(iType != EValueType::String && m_iType == EValueType::String) {
            Free(m_Value.str); // Switching from string storage, habe to free the memory
        }

        m_iType = iType;

        memset(&m_Value, 0, sizeof(m_Value));
    }

    EValueType CValue::Type() const {
        return m_iType;
    }
}
