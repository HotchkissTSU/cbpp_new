#include "cbpp/entity/CPointEntity.h"

#include "cbpp/cbdef.h"
#include "cbpp_api/Math.h"

#ifdef CBPP_SERVERSIDE

namespace cbpp {
    CPointEntity::CPointEntity() {
        ConstructProps();
    }

    Vec2f CPointEntity::Position() {
        if(m_pParent == NULL) {
            return m_vPos;
        }

        return m_vPos.Rotated(m_pParent->Angle()) + m_pParent->Position();
    }

    float CPointEntity::Angle() {
        if(m_pParent == NULL) {
            return m_fAngle;
        }

        return m_fAngle + m_pParent->Angle();
    }

    Vec2f& CPointEntity::LocalPosition() {
        return m_vPos;
    }

    float& CPointEntity::LocalAngle() {
        return m_fAngle;
    }

    Vec2f CPointEntity::Forward() const {
        return VectorFromAngle(m_fAngle);
    }

    Vec2f CPointEntity::Right() const {
        Vec2f vForward = this->Forward();
        return Vec2f( -vForward.Y(), vForward.X() ); // (x,y) -> (-y,x), 90 degree rotation
    }

    void CPointEntity::Think() {
        printf("CPointEntity thinker\n");
    }

    // PARENTING

    CPointEntity* CPointEntity::GetParent() { return m_pParent; }
    CArray<CPointEntity*>& CPointEntity::GetChildren() { return m_aChildren; }

    void CPointEntity::PushChild(CPointEntity* pChild) {
        //static const decltype(NULL) s_iNULL = NULL;
        m_aChildren.Emplace(pChild, nullptr);
    }

    void CPointEntity::RemoveChild(CPointEntity* pChild) {
        for(size_t i = 0; i < m_aChildren.Length(); i++) {
            if(pChild->UID() == m_aChildren[i]->UID()) {
                m_aChildren[i] = nullptr;
            }
        }
    }

    bool CPointEntity::HasChild(const CPointEntity* pChild) const {
        for(size_t i = 0; i < m_aChildren.Length(); i++) {
            if( pChild->UID() == m_aChildren[i]->UID() ) {
                return true;
            }
        }

        return false;
    }

    CPointEntity* CPointEntity::SetParent(CPointEntity* pNewParent) {
        if(pNewParent == NULL) {    // Deparenting completely
            m_vPos = m_pParent->Position() + m_vPos;    // Recalculate the offset, so this entity will retain it`s position after deparenting
            m_pParent->RemoveChild(this);
            CPointEntity* pOldParent = m_pParent;
            m_pParent = NULL;
            return pOldParent;
        }

        if(m_pParent != NULL) {     // Changing our parent
            m_vPos = m_pParent->Position() + m_vPos;    // (1)
            m_pParent->RemoveChild(this);
            CPointEntity* pOldParent = m_pParent;       // (1) and (2) - recalculating our offset to retain our world position after the operation
            m_pParent = pNewParent;
            pNewParent->PushChild(this);
            m_vPos = m_vPos - m_pParent->Position();    // (2)
            return pOldParent;
        }

        // Parenting for the first time

        m_pParent = pNewParent;
        m_pParent->PushChild(this);

        return NULL;
    }

    CPointEntity::~CPointEntity() {
        for(size_t i = 0; i < m_aChildren.Length(); i++) {
            m_aChildren[i]->SetParent(NULL);
        }
    }
}

#endif
