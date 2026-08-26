#include "ui/UIBase.h"

#include "containers/Stack.h"

namespace bitum::ui {
    void CWidget::PushChild(CWidget* pChild) {
        if( m_dChildren.HasKey(pChild->GetUID()) ) { return; }

        m_dChildren.Insert(pChild->GetUID(), pChild);
    }

    CWidget::CWidget(CWidget* pParent) {
        if(pParent) {
            pParent->PushChild(this);
        }

        s_iCounter++;
    }

    size_t CWidget::GetUID() const {
        return s_iCounter;
    }

    void CWidget::SetSize(Vec2f vSize) {
        m_vSize = vSize;
    }

    Vec2f CWidget::GetSize() const {
        return m_vSize;
    }

    void CWidget::SetPos(Vec2f vPos) {
        m_vPos = vPos;
    }

    Vec2f CWidget::GetPos() const {
        return m_vPos;
    }

    void CWidget::SetAlignment(EAlign iDir) {
        m_iAlignment = iDir;
    }

    void CWidget::DockChildren() const {
        if(m_dChildren.Length() == 0) { return; }

        float_t iDelta = (float_t)(m_vSize[(size_t)m_iAlignment]) / (float_t)(m_dChildren.Length());

        for( size_t i = 0; i < m_dChildren.Length(); i++ ) {
            CWidget* pCurrent = m_dChildren.Index(i);

            Vec2f vPos, vSize;

            switch(m_iAlignment) {
                case EAlign::Horizontal: {
                    vPos = Vec2f(m_vPos.x() + iDelta*i, 0.0f) + Vec2f(m_fSpacing);
                    vSize = Vec2f(iDelta, m_vSize.y()) - Vec2f(m_fSpacing*2);

                    pCurrent->SetPos(vPos);
                    pCurrent->SetSize(vSize);

                    break;
                }

                case EAlign::Vertical: {
                    vPos = Vec2f(0.0f, m_vPos.y() + iDelta*i) + Vec2f(m_fSpacing);
                    vSize = Vec2f(m_vSize.x(), iDelta) - Vec2f(m_fSpacing*2);

                    pCurrent->SetPos(vPos);
                    pCurrent->SetSize(vSize);
                    
                    break;
                }
            }
        }
    }

    size_t CWidget::GetChildrenCount() const {
        return m_dChildren.Length();
    }

    CWidget* CWidget::IndexChild(size_t index) const {
        if( index >= m_dChildren.Length() ) { return NULL; }
        return m_dChildren.Index(index);
    }

    void CWidget::Dock() const {
        stack_t<CWidget*> stack;

        stack.Push((CWidget*)this);

        while(stack.Length() > 0) {
            CWidget* pCurrent = stack.Head();
            stack.Pop();

            pCurrent->DockChildren();

            for(size_t i = 0; i < pCurrent->GetChildrenCount(); i++) {
                stack.Push(pCurrent->IndexChild(i));
            }
        }
    }
}
