#ifndef CBPP_ENT_IPROPERTY_H
#define CBPP_ENT_IPROPERTY_H

#include <stdint.h>

namespace cbpp {
    /*
        Generic type marks (for UI input mostly)
    */
    enum class EGenericType : uint16_t {
        Incompat,
        Bool,
        Integer32,
        Float,
        Vector2D,
        Vector3D,
        Color,
        String,
        Enum
    };

    class CBaseEntity;

    // Property interface
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
}

#endif
