#ifndef CBPP_ENT_TEST_H
#define CBPP_ENT_TEST_H

#include "engine/entity/CBase.h"
#include "engine/ent_utils.h"

namespace cbpp::ent {
    class CTest : public CBase {
        
        CbEntity(CTest, CBase,
            CbIntProperty(iTest)
            CbIntPropertyEx(iExTest, min_value = 0, max_value = 52)
            CbFloatPropertyEx(float_test, min_value = -15.0f, max_value = 15.0f)
        )

        public:
            void Init(Datadesc*);
            void Destruct();
    };         
}

#endif
