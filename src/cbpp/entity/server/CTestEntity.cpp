#include "cbpp/entity/CTestEntity.h"

namespace cbpp {
    CTestEntity::CTestEntity() {
        ConstructProps();
        printf("test entity created!\n");
    }

    void CTestEntity::Think() {
        CBaseEntity::Think();
        printf("CTestEntity thinker\n");
    }

    CTestEntity::~CTestEntity() {

    }
}
