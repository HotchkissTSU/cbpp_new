#ifndef CBPP_ENTITY_PROPS_H
#define CBPP_ENTITY_PROPS_H

#include <stddef.h>
#include <stdint.h>

#include "cbpp/Constants.h"

namespace cbpp {
    class IEntityProperty {
        public:
            enum EType : uint32_t {
                Number,
                String,
                AssetPath,
                Color,
                Vector
            };

        protected:
            EType m_iType = EType::Number;
            const char *m_sName, *m_sDesc;

        public:
            IEntityProperty(const char* sName, const char* sDesc);

            CBPP_PROTECTED_CLASS(IEntityProperty);

            const char* Name() const;
            const char* Desc() const;
            EType Type() const;
    };

    class CNumberEntityProperty : public IEntityProperty {
        private:
            union {
                int32_t m_iData;
                float m_fData;
            };

        public:
            CNumberEntityProperty(const char* sName, const char* sDesc);

            void SetValue(int32_t iData);
            void SetValue(float fData);
    };
}

#endif
