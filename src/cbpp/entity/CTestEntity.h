#ifndef CBPP_ENTITY_TEST_H
#define CBPP_ENTITY_TEST_H

#include "cbpp_api/Entity.h"

namespace cbpp {
    class CTestEntity final : public CBaseEntity {
        public:
            enum ETest : uint16_t {
                ETEST_A,
                ETEST_B,
                ETEST_C
            };

        private:
            ETest m_iEnum = ETEST_A;
            int32_t m_iNumber = 52;

            CbProperties {
                CbEnumProperty(m_iEnum,
                                "A", ETEST_A,
                                "B", ETEST_B,
                                "C", ETEST_C);

                CbIntProperty(m_iNumber, 0, 100, 1);
            }

        public:
            CTestEntity();

            void Think() {}
            void Render() {}
            void NetSync() {}
    };
    CbRegisterEntity(CTestEntity, "ent_test");
}

#endif
