#ifndef CBPP_ENTITY_PHYSICS_H
#define CBPP_ENTITY_PHYSICS_H

#include "entity/CPointEntity.h"

namespace cbpp {
    class CPhysicsEntity : public CPointEntity {
        private:
            uint32_t m_iMass;
            Vec2f m_vAccel, m_vVel;
            float m_fAngAccel, m_fAngVel;

        public:
            CbProperties {
                CbPropertyEx(m_iMass, "iMass", "#entity.CPhysicsEntity.mass", "#entity.CPhysicsEntity.mass_desc", EGenericType::Integer32);
            }

            CPhysicsEntity();

            uint32_t Mass() const;
            Vec2f Velocity() const;
            Vec2f Acceleration() const;
            float KineticEnergy() const;

            void ApplyForce(Vec2f vForce);
            void ApplyForceOffset(Vec2f vForce, Vec2f vOffset);

            virtual ~CPhysicsEntity() override = default;
    };
    CbAbstractEntity(CPhysicsEntity, "base_phys");
}

#endif
