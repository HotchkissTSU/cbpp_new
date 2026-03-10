#ifndef CBPP_ENT_TEST_H
#define CBPP_ENT_TEST_H

#include "engine/entity/CBase.h"
#include "engine/ent_utils.h"

namespace cbpp::ent {
    class CTest : public CBase {
        CbEntity(CTest, CBase, NULL)

        public:
            void Init(Datadesc*);
            void Destruct();
    };         
}

#endif
