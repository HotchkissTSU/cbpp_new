#ifndef CBPP_ENTITY_HEALTH_H
#define CBPP_ENTITY_HEALTH_H

#include "entity/CBaseEntity.h"

namespace cbpp {
    class CHealthEntity;

    enum class EDamageType : uint16_t {
        Generic,        // Anything uncommon
        Bullet,         // Boolet
        Blunt,          // Blunt melee hit (sledgehammer)
        Sharp           // Sharp melee hit (knife / spade)
    }; 

    struct DamageInfo {
        CBaseEntity* pDamager = NULL;
        CBaseEntity* pVictim = NULL;
        EDamageType iDmgType = EDamageType::Generic;
        float fBaseDamage = 0.0f;
    };

    class CHealthEntity : public CBaseEntity {
        float m_iHealth, m_iMaxHealth;

        public:
            CbProperties {
                CbPropertyEx(m_iHealth,     "iHealth", 
                                            "#entity.CHealthEntity.health", 
                                            "#entity.CHealthEntity.health_desc", 
                                            EGenericType::Float);

                CbPropertyEx(m_iMaxHealth,  "iMaxHealth", 
                                            "#entity.CHealthEntity.max_health", 
                                            "#entity.CHealthEntity.max_health_desc", 
                                            EGenericType::Float);
            }

            CHealthEntity() { ConstructProps(); }

            bool IsAlive() const;
            void ApplyDamage(float fDmg);
            float Health() const;
            float MaxHealth() const;
            void SetHealth();

            virtual void OnHurt(DamageInfo Dmg) = 0;

            virtual ~CHealthEntity() override = default;
    };
    CbAbstractEntity(CHealthEntity, "base_health");
}

#endif
