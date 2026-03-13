#ifndef CBPP_ENTITY_PROPS_H
#define CBPP_ENTITY_PROPS_H

#include <stddef.h>
#include <stdint.h>

#include "cbpp/Constants.h"
#include "cbpp/Vector.h"
#include "cbpp/String.h"
#include "cbpp/Array.h"
#include "cbpp/Image.h"

namespace cbpp {
    namespace ent {
        enum class EStringType : uint32_t {
            String,                             // A generic string info
            EntityName,                         // Name of another entity
            GenericPath,                        // Path to something
            ModelPath,                          // Path to a model file
            SoundPath                           // Path to a sound file
        };
    }

    class IEntityProperty {
        public:
            enum class EType : uint32_t {
                Integer,                        // 32 bit signed integer
                Float,                          // 32 bit float
                String,                         // Any string data
                Vector,                         // Two floats combined
                Color,                          // 4 bytes of RGBA
                Enum,                           // Enumeration
                Bool                            // Boolean value
            };

            static const char* GetTypeName(EType iType);

        protected:
            EType m_iType = EType::Integer;
            const char *m_sName;

        public:
            IEntityProperty(const char* sName, EType iType) : m_sName(sName), m_iType(iType) {}

            CBPP_PROTECTED_CLASS(IEntityProperty);

            const char* Name() const;
            EType Type() const;

            virtual ~IEntityProperty() = default;
    };

    template <typename T> class CEntityPropertyStorage : public IEntityProperty {
        protected:
            T* m_pData;

        public:
            CEntityPropertyStorage(const char* sName, EType iType, T* pData) : IEntityProperty(sName, iType), m_pData(pData) {}

            T* Address() { return m_pData; }
            size_t Sizeof() { return sizeof(T); }

            virtual ~CEntityPropertyStorage() = default;
    };

    class CNumberEntityProperty : public CEntityPropertyStorage<int32_t> {
        private:
            const int32_t m_iMin, m_iMax;

        public:
            CNumberEntityProperty(const char* sName, int32_t* pData, int32_t iDefault = 0, int32_t iMin = INT32_MIN, int32_t iMax = INT32_MAX) : 
                CEntityPropertyStorage<int32_t>(sName, EType::Integer, pData), m_iMin(iMin), m_iMax(iMax) {SetValue(iDefault);}

            Vec2i Limits();
            void SetValue(int32_t iValue);
            int32_t GetValue();

            virtual ~CNumberEntityProperty() = default;
    };

    class CFloatEntityProperty : public CEntityPropertyStorage<float> {
        private:
            const float m_fMin, m_fMax;

        public:
            CFloatEntityProperty(const char* sName, float* pData, float fDefault = 0.0f, float fMin = __FLT_MIN__, float fMax = __FLT_MAX__) : 
                CEntityPropertyStorage<float>(sName, EType::Float, pData), m_fMin(fMin), m_fMax(fMax) {SetValue(fDefault);}

            Vec2f Limits();
            void SetValue(float iValue);
            float GetValue();

            virtual ~CFloatEntityProperty() = default;
    };

    class CStringEntityProperty : public CEntityPropertyStorage<CString> {
        private:
            ent::EStringType m_iStringType;
        public:
            CStringEntityProperty(const char* sName, CString* pData, const char* default_value = "", ent::EStringType string_type = ent::EStringType::String) :
                CEntityPropertyStorage<CString>(sName, EType::String, pData), m_iStringType(string_type) { pData->Set(default_value); }

            void SetValue(const char* sData);
            void SetValue(const CString& sData);

            const char* GetValue();
            size_t Length();

            virtual ~CStringEntityProperty() = default;
    };

    class CVectorEntityProperty : public CEntityPropertyStorage<Vec2f> {
        private:
            const Vec2f m_vMin, m_vMax;

        public:
            CVectorEntityProperty(const char* sName, Vec2f* pData, Vec2f vDefault = {0.0f, 0.0f}, 
                Vec2f vMin = {__FLT_MIN__,__FLT_MIN__}, Vec2f vMax = {__FLT_MAX__,__FLT_MAX__}) : 
                CEntityPropertyStorage<Vec2f>(sName, EType::Vector, pData), m_vMax(vMax), m_vMin(vMin)
            {
                this->SetValue(vDefault);
            }

            void SetValue(Vec2f vValue);
            Vec2f GetValue();

            Vec2f MinBound();
            Vec2f MaxBound();

            virtual ~CVectorEntityProperty() = default;
    };

    class CColorEntityProperty : public CEntityPropertyStorage<Color> {
        public:
            CColorEntityProperty(const char* sName, Color* pData, Color Default = {(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)255}) :
                CEntityPropertyStorage<Color>(sName, EType::Color, pData) { this->SetValue(Default); }

            Color GetValue();
            void SetValue(Color Data);

            virtual ~CColorEntityProperty() = default;
    };

    class CBoolEntityProperty : public CEntityPropertyStorage<bool> {
        public:
            CBoolEntityProperty(const char* sName, bool* pData, bool bDefault = true) :
                CEntityPropertyStorage<bool>(sName, EType::Bool, pData) { *m_pData = bDefault; }

            void SetValue(bool bValue);
            bool GetValue();

            virtual ~CBoolEntityProperty() = default;
    };

    class CEnumEntityProperty : public CEntityPropertyStorage<int32_t> {
        public:
            struct Pair {
                const char* Name;
                int32_t Value;
            };
            
        private:
            CArray<Pair> m_aPairs;

            void RegisterPair() {
                m_aPairs.Shrink();
            }

            template <typename enum_t, typename ... Args> void RegisterPair(const char* sName, enum_t iValue, Args... Pairs) {
                m_aPairs.PushBack({sName, (int32_t)iValue});
                RegisterPair(Pairs...);
            }
            
        public:
            template <typename ... Args> CEnumEntityProperty(const char* sName, int32_t* pData, int32_t default_value = 0, Args... Pairs) :
                CEntityPropertyStorage<int32_t>(sName, EType::Enum, pData) // i am insane
            {
                *pData = default_value;
                RegisterPair(Pairs...);
            }

            size_t PairsAmount() {
                return m_aPairs.Length();
            }

            const Pair* GetPairs() {
                return (const Pair*)(m_aPairs.Data());
            }

            int32_t GetValue() {
                return *m_pData;
            }

            /*
                Templated for automatic explicit cast from enum type to the
                underlying int32_t, because Retard++ can`t do it himself
            */
            template <typename enum_t> void SetValue(enum_t iValue) {
                *m_pData = (int32_t)iValue;
            }

            virtual ~CEnumEntityProperty() = default;
    };
}

#endif
