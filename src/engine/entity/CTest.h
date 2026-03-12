#ifndef CBPP_ENT_TEST_H
#define CBPP_ENT_TEST_H

#include "engine/entity/CBase.h"
#include "engine/ent_utils.h"

namespace cbpp::ent {
    class CTest : public CBase {
        
        /*
            Drop this macro to turn your class into the game entity
        */
        CbEntity(CTest, CBase,
            CbIntProperty(iTest)
            CbIntPropertyEx(iExTest, 5, 0, 52)                                          // Describe some properties
            CbFloatPropertyEx(float_test, 0, -15, 15)
            CbStringPropertyEx(string_test, "default string", EStringType::String)
            CbEnumProperty(enum_test,   "VALUE_A", 0,
                                        "VALUE_B", 1,
                                        "VALUE_C", 2)
        )

        public:
            void Init(Datadesc*);       // Entities have these callbacks for construction and destruction
            void Destruct();
    };         
}

#endif
