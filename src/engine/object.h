#ifndef CBPP_OBJECT_H
#define CBPP_OBJECT_H

/*
    Generic "object" interface, meant to be used in CDF, CML, and possibly other formats later
*/

#include <stdint.h>

#include "cbpp/Constants.h"

namespace cbpp {
    class IObject {
        public:
            enum class EType : int32_t {
                Primitive,
                Array,
                Table,
                User
            };
        
        protected:
            const EType m_iType;
            const uint32_t m_iUserType;
            
            IObject(EType iType, uint32_t iUserType);

            CBPP_PROTECTED_CLASS(IObject);

        public:
            virtual EType Type() const = 0;
            virtual uint32_t UserType() const = 0;
    };
}

#endif
