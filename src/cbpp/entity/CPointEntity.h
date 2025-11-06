/*
    A basis for all entities with position and angle.
    Also implements real time parenting - child entities exist in a local space relative to their parent
*/

#ifndef CBPP_ENTITY_POINT_H
#define CBPP_ENTITY_POINT_H

#include "cbpp/entity/CBaseEntity.h"
#include "cbpp/vector.h"
#include "cbpp_api/Array.h"

namespace cbpp {
    class CPointEntity;

    class CPointEntity : public CBaseEntity {
        Vec2f m_vPos;
        float m_fAngle;

        CPointEntity* m_pParent = NULL;
        CArray<CPointEntity*> m_aChildren;

        protected:
            void PushChild(CPointEntity* pChild);
            void RemoveChild(CPointEntity* pChild);

        public:
            CbProperties {
                CbArrayPropertyEx(m_vPos,   "vPosition", 
                                            "#entity.CPointEntity.position", 
                                            "#entity.CPointEntity.position_desc", 
                                            EGenericType::Float, 2);

                CbPropertyEx(m_fAngle,      "fAngle", 
                                            "#entity.CPointEntity.angle", 
                                            "#entity.CPointEntity.angle_desc", 
                                            EGenericType::Float);
            }
        
            virtual void Think() override;
            virtual void Render() override = 0;
            virtual void NetSync() override = 0;

            CPointEntity();

            CPointEntity* GetParent();

            /*
                Set a new parent entity (or deparent, if input is NULL)
                Returns the previous parent or NULL if none
            */
            CPointEntity* SetParent(CPointEntity* pNewParent);

            CArray<CPointEntity*>& GetChildren();
            bool HasChild(const CPointEntity* pChild) const;

            // Get the forward vector of the entity
            Vec2f Forward() const;

            // Get the right vector of the entity
            Vec2f Right() const;

            Vec2f& LocalPosition();
            float& LocalAngle();
            
            Vec2f Position();
            float Angle();

            virtual ~CPointEntity() override;
    };
    CbAbstractEntity(CPointEntity, "base_point");
}

#endif
