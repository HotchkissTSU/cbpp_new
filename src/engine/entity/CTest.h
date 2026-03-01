#ifndef CBPP_ENT_TEST_H
#define CBPP_ENT_TEST_H

#include "engine/entity/CBase.h"
#include "engine/ent_utils.h"

namespace cbpp::ent {
    class CTest : public CBase {
        CbProperties (
            NULL,
            NULL
        )

        public:
            virtual ~CTest() = default;
    };
    CbRegisterEntity(CTest);
}

#endif
