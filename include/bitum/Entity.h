#ifndef BITUM_API_ENTITY_H
#define BITUM_API_ENTITY_H

#include "Meta.h"

namespace bitum {
    class IEntityProperty {
        public:
            virtual meta::EType Type() const = 0;
            virtual void* Address() = 0;
    };

    class IEntityData {
        public:
            virtual const char* ClassName() const = 0;
            virtual size_t ClassIndex() const = 0;

            virtual IEntityProperty* IndexProperty(size_t) = 0;
    };

    class IEntity {
        public:
            virtual const char* ClassName() const = 0;
            virtual size_t ClassIndex() const = 0;
    };
};

#endif
