/*
    Properties implementation
*/

#ifndef CBPP_ENTPROPS_H
#define CBPP_ENTPROPS_H

#include <stddef.h>
#include <stdint.h>

#include "cbpp/Memory.h"

#include "cbpp/Array.h"
#include "cbpp/Filesystem.h"

#include "engine/entity/CBaseEntity.h"

#define CbIntProperty(_member, _min, _max, _arrlen)\
    cbpp::New<CIntProperty>(this, #_member, &_member, sizeof(_member), _arrlen, _min, _max)

#define CbFloatProperty(_member, _min, _max, _arrlen)\
    cbpp::New<CFloatProperty>(this, #_member, &_member, sizeof(_member), _arrlen, _min, _max)

#define CbBoolProperty(_member)\
    cbpp::New<CBoolProperty>(this, #_member, &_member, sizeof(_member))

#define CbStringProperty(_member)\
    cbpp::New<CStringProperty>(this, #_member, &_member, sizeof(_member), EAsserPath::Invalid)

#define CbAssetPathProperty(_member, _asset_grp)\
    cbpp::New<CStringProperty>(this, #_member, &_member, sizeof(_member), _asset_grp)

#define CbVector2Property(_member)\
    cbpp::New<CFloatProperty>(this, #_member, (float*)(&_member), sizeof(_member), 2, __FLT_MIN__, __FLT_MAX__)

#define CbVector3Property(_member)\
    cbpp::New<CFloatProperty>(this, #_member, (float*)(&_member), sizeof(_member), 3, __FLT_MIN__, __FLT_MAX__)

#define CbEnumProperty(_member, ...)\
    cbpp::New<CEnumProperty>(this, #_member, &_member, sizeof(_member), __VA_ARGS__)

// Start describing this entity`s properties
#define CbProperties virtual const char* Class() const; virtual bool IsAbstract() const; virtual void ConstructProps()

namespace cbpp {
    // Property storage
    template <typename T> class CBaseProperty : public IProperty {
        protected:
            T* m_pData;
            size_t m_iLength = 1;
            EGenericType m_iType = EGenericType::Bool;

        public:
            CBaseProperty(CBaseEntity* eMaster, const char* sName, T* pData, size_t iSize, size_t iLength, EGenericType iType) : 
                        m_pData(pData), m_iType(iType), m_iLength(iLength), IProperty(eMaster, sName) 
            {
                const size_t iExpect = sizeof(T) * iLength;
                CbAssertf(iExpect > iSize, "Too small '%s.%s' property buffer size: expected %zu, got %zu", eMaster->Class(), sName, iExpect, iSize);
            }

            T* GetBuffer() {
                return m_pData;
            }

            size_t Sizeof() {
                return sizeof(T);
            }

            T* Index(size_t iIndex) {
                if(iIndex > m_iLength) {
                    return m_pData;
                }

                return &m_pData[iIndex];
            }

            size_t Length() {
                return m_iLength;
            }

            EGenericType Type() {
                return m_iType;
            }

            virtual ~CBaseProperty() override = default;
    };

    // Property implementations with custom behaviour

    class CIntProperty final : public CBaseProperty<int32_t> {
        typedef CBaseProperty<int32_t> container_t;

        const int32_t m_iMin = 0, m_iMax = 100;

        public:
            CIntProperty(CBaseEntity* eMaster, const char* sName, int32_t* pValue, size_t iSize, size_t iLength, 
                                                                        int32_t iMin, int32_t iMax) : 
                                                                        m_iMin(iMin), 
                                                                        m_iMax(iMax), 
                                                                        container_t(eMaster, sName, pValue, iSize, iLength, EGenericType::Integer32) {}

            void SetValue(int32_t iValue);
            int32_t GetValue();

            virtual ~CIntProperty() override = default;
    };

    class CFloatProperty final : public CBaseProperty<float> {
        typedef CBaseProperty<float> container_t;

        const float m_fMin = 0.0f, m_fMax = 100.0f;

        public:
            CFloatProperty(CBaseEntity* eMaster, const char* sName, float* pValue, size_t iSize, size_t iLength, float fMin, float fMax) : 
                                                                        m_fMin(fMin), 
                                                                        m_fMax(fMax), 
                                                                        container_t(eMaster, sName, pValue, iSize, iLength, EGenericType::Float) {}

            void SetValue(float iValue);
            float GetValue();

            virtual ~CFloatProperty() override = default;
    };

    class CBoolProperty final : public CBaseProperty<bool> {
        typedef CBaseProperty<bool> container_t;

        public:
            CBoolProperty(CBaseEntity* eMaster, const char* sName, bool* pValue, size_t iSize) :
                                                                        container_t(eMaster, sName, pValue, iSize, 1, EGenericType::Bool) {}

            void SetValue(bool bValue);
            bool GetValue();

            virtual ~CBoolProperty() = default;
    };

    class CStringProperty final : public CBaseProperty<CString> {
        typedef CBaseProperty<CString> container_t;

        EAssetPath m_iAssetGroup = EAssetPath::Invalid;

        public:
            CStringProperty(CBaseEntity* eMaster, const char* sName, CString* pData, size_t iSize, EAssetPath iAssetGrp) : 
                            container_t(eMaster, sName, pData, iSize, 1, EGenericType::String), m_iAssetGroup(iAssetGrp) {}

            bool IsAssetPath() const;

            virtual ~CStringProperty() = default;
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
            struct Reg {
                uint16_t m_iCounter = 0;
                pairs_t m_aPairs;

                void ProcessPair(const char*, const char*) {};

                template <typename value_t, typename... args_t> 
                void ProcessPair(const char* sMasterClass, const char* sPropName, const char* sName, value_t iValue, args_t... Args) {
                    char sLocKeyBuff[128];

                    snprintf(sLocKeyBuff, sizeof(sLocKeyBuff), "#entity.%s.%s.%s", sMasterClass, sPropName, sName);
                    //printf("Enum '%s' entry -> '%s' (name)\n", sPropName, sLocKeyBuff);

                    snprintf(sLocKeyBuff, sizeof(sLocKeyBuff), "#entity.%s.%s.%s_desc", sMasterClass, sPropName, sName);
                    //printf("Enum '%s' entry -> '%s' (desc)\n", sPropName, sLocKeyBuff);

                    Pair Pair { sName, (uint16_t)iValue };
                    m_aPairs.PushBack(Pair);
                    ProcessPair(sMasterClass, sPropName, Args...);
                    m_iCounter++;
                }

                template <typename... args_t> Reg(const char* sMasterClass, const char* sPropName, args_t... Args) {
                    ProcessPair(sMasterClass, sPropName, Args...);
                    m_aPairs.Shrink();
                }
            };

            Reg* m_pRegistry;

        // This constructor just has to take void* as a member pointer argument, because Retard++ cant 
        // perform an implicit cast from the enum type to the integer

        public:
            template <typename... args_t> CEnumProperty(CBaseEntity* eMaster, const char* sName, void* pData, size_t iSize, args_t... Args) :
            container_t(eMaster, sName, (uint16_t*)pData, iSize, 1, EGenericType::Enum)
            {
                static Reg s_PairsReg(eMaster->Class(), m_sName, Args...); // we do only construct this list once for each enumeration
                m_pRegistry = &s_PairsReg;
            }

            void SetValue(uint16_t iValue);
            uint16_t GetValue();

            const pairs_t& GetPairs() const;

            virtual ~CEnumProperty() = default;
    };
}

#endif
