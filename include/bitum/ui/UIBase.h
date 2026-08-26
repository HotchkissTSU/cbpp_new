#ifndef BITUM_UI_BASE_H
#define BITUM_UI_BASE_H

#include "containers/Table.h"

#include "Vector.h"

namespace bitum::ui {
    enum class EAlign : uint32_t {
        Horizontal  = 0,
        Vertical    = 1
    };

    class CWidget {
        static size_t s_iCounter;

        Vec2f m_vPos, m_vSize;

        CWidget* m_pParent = NULL;
        table_t<size_t, CWidget*> m_dChildren;

        EAlign m_iAlignment = EAlign::Vertical;
        float_t m_fSpacing = 0.0f;

        void PushChild(CWidget*);

        public:
            CWidget(CWidget* pParent);

            size_t GetUID() const;

            void SetSize(Vec2f);
            Vec2f GetSize() const;

            void SetPos(Vec2f);
            Vec2f GetPos() const;

            /*
                Dock this widget`s children to match the parent size
            */
            void DockChildren() const;

            /*
                Recursively dock all of the children
            */
            void Dock() const;

            void SetAlignment(EAlign);

            size_t GetChildrenCount() const;
            CWidget* IndexChild(size_t) const;
    };
}

#endif
