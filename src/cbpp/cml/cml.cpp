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

    CValue::CValue(const CString& sValue) {
        m_iType = EValueType::String;
        m_Value.str = StringDup(sValue.String());
    }

    CValue::~CValue() {
        if(m_iType == EValueType::String) {
            Free(m_Value.str);
        }
    }
}
