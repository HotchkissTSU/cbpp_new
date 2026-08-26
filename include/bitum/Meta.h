#ifndef BITUM_METACLASS_H
#define BITUM_METACLASS_H

#include <stdint.h>

#include "containers/Array.h"

namespace bitum::meta {
    enum EType : uint32_t {
        INT,
        FLOAT,
        STRING,
        BOOL
    };

    struct SFieldInfo {
        const char* name;
        void* address;
        EType type;
    };

    using CMetaFields = array_t<SFieldInfo>;

    class IMetaclass {
        public:
            virtual const CMetaFields& GetFields() const = 0;
    };
}

#endif
