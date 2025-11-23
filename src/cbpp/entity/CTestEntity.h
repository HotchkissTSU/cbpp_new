#ifndef CBPP_ENTITY_TEST_H
#define CBPP_ENTITY_TEST_H

/*
    Entity creation example
*/

#include "cbpp_api/Entity.h"

namespace cbpp {
    class CTestEntity final : public CBaseEntity {          // 1) Derive from CBaseEntity
        public:
            enum ETest : uint16_t {
                ETEST_A,
                ETEST_B,
                ETEST_C
            };

        private:
            ETest m_iEnum = ETEST_A;
            int32_t m_iNumber = 52;
            float m_aVec3[3];

            CbProperties {                                  // 2) Describe some properties, or leave an empty "CbProperties {}"
                CbEnumProperty(m_iEnum,
                                "A", ETEST_A,
                                "B", ETEST_B,
                                "C", ETEST_C);

                CbIntProperty(m_iNumber, 0, 100, 1);
                CbVector3Property(m_aVec3);
            }

        public:
            CTestEntity();                                  // 3) Dont forget to call "CTestEntity::ConstructProps()" in our constructor

            void Think() {}                                 // 4) Call parent entity`s thinker callback in our thinker
            void Render() {}                                // 5) Provide these 3 callbacks
            void NetSync() {}
    };
    CbRegisterEntity(CTestEntity, "ent_test");              // 6) Register our entity
}

#endif
