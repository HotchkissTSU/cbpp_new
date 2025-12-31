#include "cbpp/cml/cml.h"

namespace cbpp::cml {
    #define KW(_str) if( strcmp(sData, _str) == 0 ) { return true; }
    bool IsKeyword(const char* sData) {
        if(IsTypename(sData)) { return true; }

        KW("include")

        return false;
    }
    
    bool IsTypename(const char* sData) {
        KW("int")
        KW("float")
        KW("bool")
        KW("string")
        KW("dict")
        KW("vec2")

        return false;
    }
}
