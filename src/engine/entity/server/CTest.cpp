#include "engine/entity/CTest.h"

namespace cbpp::ent {
    void CTest::Construct() {
        puts("CTest construct!");
    }

    void CTest::Init(Datadesc* pData) {
        puts("CTest init!");
    }

    void CTest::Destruct() {}

    void CSubTest::Construct() {
        puts("CSubTest construct!");
    }

    void CSubTest::Init(Datadesc* p) {
        puts("CSubTest init!");
    }

    void CSubTest::Destruct() {}
}
