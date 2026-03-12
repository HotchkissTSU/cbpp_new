#ifndef CBPP_ENT_TEST_H
#define CBPP_ENT_TEST_H

#include "engine/entity/CBase.h"
#include "engine/ent_utils.h"

namespace cbpp::ent {
    enum class Dolboeb {
        A, B, C, D
    };

    class CTest : public CBase {
        
        /*
            Drop this macro to turn your class into the game entity
        */
        CbEntity(CTest, CBase,
            CbIntProperty(iTest)
            CbIntPropertyEx(iExTest, 5, 0, 52)                                          // Describe some properties
            CbFloatPropertyEx(float_test, 0, -15, 15)
            CbStringPropertyEx(string_test, "default string", EStringType::String)
            CbEnumProperty(enum_test,   "VALUE_A", Dolboeb::A,
                                        "VALUE_B", Dolboeb::B,
                                        "VALUE_C", Dolboeb::C)
        )

        public:
            virtual void Construct();               // Entity constructor
            virtual void Init(Datadesc*);           // Initialization function (called by cbpp::InitEntity(...))
            virtual void Destruct();                // Entity destructor
    };      

    class CSubTest : public CTest {
        CbEntity(CSubTest, CTest,
            CbIntProperty(test_int)

            CbVectorPropertyEx(vector2, 5,10, -1,-1, 1,1)

            CbColorPropertyEx(color_test, 255u,0u,0u,255u)
        )

        public:
            virtual void Construct();
            virtual void Init(Datadesc*);
            virtual void Destruct();   
    };   
}

#endif
