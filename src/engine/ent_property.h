#ifndef CBPP_ENTITY_PROPS_H
#define CBPP_ENTITY_PROPS_H

#include <stddef.h>
#include <stdint.h>

#include "cbpp/Constants.h"
#include "cbpp/Vector.h"

namespace cbpp {
    class IEntityProperty {
        public:
            enum class EType : uint32_t {
                Integer,
                Float,
                String,
                Vector,
                Color,
                Enum
            };

        protected:
            EType m_iType = EType::Integer;
            const char *m_sName;

        public:
            IEntityProperty(const char* sName, EType iType) : m_sName(sName), m_iType(iType) {}

            CBPP_PROTECTED_CLASS(IEntityProperty);

            const char* Name() const;
            EType Type() const;
    };

    template <typename T> class CEntityPropertyStorage : public IEntityProperty {
        protected:
            T* m_pData;

        public:
            CEntityPropertyStorage(const char* sName, EType iType, T* pData) : IEntityProperty(sName, iType), m_pData(pData) {}

            T* Address() { return m_pData; }
            size_t Sizeof() { return sizeof(T); }
    };

    class CNumberEntityProperty : public CEntityPropertyStorage<int32_t> {
        private:
            const int32_t m_iMin, m_iMax;

        public:
            CNumberEntityProperty(const char* sName, int32_t* pData, int32_t min_value = INT32_MIN, int32_t max_value = INT32_MAX) : 
                CEntityPropertyStorage<int32_t>(sName, EType::Integer, pData), m_iMin(min_value), m_iMax(max_value) {}

            Vec2i Limits();
            void SetValue(int32_t iValue);
            int32_t GetValue();
    };

    class CFloatEntityProperty : public CEntityPropertyStorage<float> {
        private:
            const float m_fMin, m_fMax;

        public:
            CFloatEntityProperty(const char* sName, float* pData, int32_t min_value = __FLT_MAX__, int32_t max_value = __FLT_MIN__) : 
                CEntityPropertyStorage<float>(sName, EType::Float, pData), m_fMin(min_value), m_fMax(max_value) {}

            Vec2f Limits();
            void SetValue(float iValue);
            float GetValue();
    };
}

#endif
