#ifndef CBPP_ENTITY_TEST_H
#define CBPP_ENTITY_TEST_H

#include "cbpp/entity/CBaseEntity.h"

namespace cbpp {
    class CTestEntity : public CBaseEntity {
        float m_fNumber1 = 10.0f;
        float m_fNumber2 = 20.0f;

        public:
            CTestEntity();
            ~CTestEntity();

            void Think() {}
            void Render() {}

            CbProperties {
                CbProperty(m_fNumber1, "test_fNum1", "bebra", EGenericType::Float);
                CbProperty(m_fNumber2, "test_fNum2", "amogus", EGenericType::Float);
            }
    };
    CbRegisterEntity(CTestEntity, "test_entity");
}

#endif
