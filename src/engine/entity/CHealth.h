#ifndef CBPP_ENT_HEALTH_H
#define CBPP_ENT_HEALTH_H

#include "cbpp/Entity.h"
#include "cbpp/Network.h"

namespace cbpp::ent {
    enum class EDamageType : uint32_t {
        Blunt,
        Cut,
        Bullet,
        Blastwave,

        AMOUNT
    };
    
    struct DamageInfo {
        ent::CBase *pInflictor, *pVictim;
        EDamageType iDamageType;
        float fBaseValue;
    };

    class CHealth : public CBase {
        CbEntity(CHealth, CBase,
            CbFloatPropertyEx(MaxHealth, 100, 0, __FLT_MAX__)
            CbFloatPropertyEx(StartHealth, 100, 0, __FLT_MAX__)
            CbStringPropertyEx(ArmorClass, "", EStringType::String)
        )

        protected:
            float m_fMaxHealth, m_fHealth;

        public:
            virtual void Construct();
            virtual void Init(Datadesc*);
            virtual void Destruct();

            virtual void OnTakeDamage(DamageInfo) {};

            float Health();
            float MaxHealth();
            bool IsAlive();

            void SetHealth(float fHealth);
            void SetMaxHealth(float fMaxHealth);

            void ApplyDamage(DamageInfo Info);

    };
}

#endif
