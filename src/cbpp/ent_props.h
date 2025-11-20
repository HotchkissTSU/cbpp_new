/*
    Entity properties

    Allow marking certain class members, so they will be visible and editable in runtime
*/

#ifndef CBPP_ENTPROPS_H
#define CBPP_ENTPROPS_H

#include <stddef.h>
#include <stdint.h>

#include "cbpp/mutable_string.h"
#include "cbpp/malloc_wrapper.h"

#include "cbpp_api/Array.h"

#define CBPP_DEFAULT_PROPERTY_DESC "Default description"

#define CbIntProperty(_member, _min, _max, _arrlen)\
    cbpp::New<CIntProperty>(this, #_member, &_member, _arrlen, _min, _max);

#define CbFloatProperty(_member, _min, _max, _arrlen)\
    cbpp::New<CFloatProperty>(this, #_member, &_member, _arrlen, _min, _max);

#define CbStringProperty(_member)\
    cbpp::New<CStringProperty>(this, #_member, &_member);

#define CbEnumProperty(_member, ...)\
    cbpp::New<CEnumProperty>(this, #_member, &_member, __VA_ARGS__);

#define CbProperties virtual const char* Class() const; virtual bool IsAbstract() const; virtual void ConstructProps()

namespace cbpp {
    /*
        Generic type marks (for UI input mostly)
    */
    enum class EGenericType : uint16_t {
        Incompat,
        Byte,
        Bool,
        Integer32,
        Float,
        Vector2D,
        Vector3D,
        Color,
        String,
        AssetPath,
        Enum
    };

    class CBaseEntity;

    class IProperty {
        protected:
            IProperty* m_pNext = NULL;
            CBaseEntity* m_eMaster = NULL;
            const char *m_sName = NULL;
        public:
            IProperty(CBaseEntity* eMaster, const char* sName);

            IProperty* Next();
            void SetNextNode(IProperty* pNext);

            const char* Name();
            const char* PrettyName();
            const char* Description();

            virtual EGenericType Type() = 0;

            CBaseEntity* Master();

            virtual ~IProperty();
    };

    // Property storage
    template <typename T> class CBaseProperty : public IProperty {
        protected:
            T* m_pData;
            size_t m_iLength = 1;
            EGenericType m_iType = EGenericType::Bool;

        public:
            CBaseProperty(CBaseEntity* eMaster, const char* sName, T* pData, size_t iLength, EGenericType iType) : 
                        m_pData(pData), m_iType(iType), m_iLength(iLength), IProperty(eMaster, sName) {}

            void* GetBuffer() {
                return (void*)(m_pData);
            }

            size_t Sizeof() {
                return sizeof(T);
            }

            void* Index(size_t iIndex) {
                if(iIndex > m_iLength) {
                    return (void*)m_pData;
                }

                return (void*)(&m_pData[iIndex]);
            }

            size_t Length() {
                return m_iLength;
            }

            EGenericType Type() {
                return m_iType;
            }

            virtual void SetValue(T Value) = 0;
            virtual T GetValue() = 0;

            virtual ~CBaseProperty() override = default;
    };

    class CIntProperty final : public CBaseProperty<int32_t> {
        typedef CBaseProperty<int32_t> container_t;

        const int32_t m_iMin = 0, m_iMax = 100;

        public:
            CIntProperty(CBaseEntity* eMaster, const char* sName, int32_t* pValue, size_t iLength, 
                                                                        int32_t iMin, int32_t iMax) : 
                                                                        m_iMin(iMin), 
                                                                        m_iMax(iMax), 
                                                                        container_t(eMaster, sName, pValue, iLength, EGenericType::Integer32) {}

            void SetValue(int32_t iValue);
            int32_t GetValue();

            virtual ~CIntProperty() override = default;
    };

    class CFloatProperty final : public CBaseProperty<float> {
        typedef CBaseProperty<float> container_t;

        const float m_fMin = 0.0f, m_fMax = 100.0f;

        public:
            CFloatProperty(CBaseEntity* eMaster, const char* sName, float* pValue, size_t iLength, float fMin, float fMax) : 
                                                                        m_fMin(fMin), 
                                                                        m_fMax(fMax), 
                                                                        container_t(eMaster, sName, pValue, iLength, EGenericType::Float) {}

            void SetValue(float iValue);
            float GetValue();

            virtual ~CFloatProperty() override = default;
    };

    class CStringProperty final : public CBaseProperty<CString> {
        typedef CBaseProperty<CString> container_t;

        public:
            CStringProperty(CBaseEntity* eMaster, const char* sName, CString* pData) : 
                            container_t(eMaster, sName, pData, 1, EGenericType::String) {}
    };

    class CEnumProperty final : public CBaseProperty<uint16_t> {
        typedef CBaseProperty<uint16_t> container_t;

        public:
            struct Pair {
                const char* sName;
                uint16_t iValue;
            };

            typedef CArray<Pair> pairs_t;

        private:
            uint16_t m_iCounter = 0;
            pairs_t m_aPairs;

            void ProcessPair() {};

            template <typename... args_t> void ProcessPair(const char* sName, uint16_t iValue, args_t... Args) {
                Pair Pair { sName, iValue };
                m_aPairs.PushBack(Pair);
                ProcessPair(Args...);
                m_iCounter++;
            }

        public:
            template <typename... args_t> CEnumProperty(CBaseEntity* eMaster, const char* sName, uint16_t* pData, args_t... Args) :
            container_t(eMaster, sName, pData, 1, EGenericType::Enum)
            {
                ProcessPair(Args...);
                m_aPairs.Shrink();
            }

            void SetValue(uint16_t iValue);
            uint16_t GetValue();

            const pairs_t& GetPairs() const;
    };
}

#endif
