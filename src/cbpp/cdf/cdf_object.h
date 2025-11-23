#ifndef CBPP_CDF_OBJECT_H
#define CBPP_CDF_OBJECT_H

#include <stddef.h>
#include <stdint.h>

namespace cbpp::cdf {
    enum class EObjectClass : uint16_t {

    };

    class IObject {
        public:
            virtual size_t Sizeof() const = 0;
            virtual size_t Length() const = 0;
            virtual EObjectClass Class() const = 0;
    };
}

#endif
