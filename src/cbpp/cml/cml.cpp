#include "cbpp/cml/cml.h"

namespace cbpp::cml {
    /*CTokenValue::CTokenValue(EToken iType, const char* sData) {
        switch (iType) {
            case EToken::Number:
                Value.F32 = atof(sData);
                break;

            case EToken::String:
                Value.S = sData;
                break;
        }
    }

    CTokenValue::ValueType& CTokenValue::GetValue() { return Value; }*/

    #define KW(_str) if( strcmp(sData, _str) == 0 ) { return true; }
    bool IsKeyword(const char* sData) {
        KW("int")
        KW("float")
        KW("bool")
        KW("string")
        KW("dict")
        KW("vec2")

        return false;
    }
}
