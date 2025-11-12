#ifndef CBPP_STATICMAP_H
#define CBPP_STATICMAP_H

#include <stddef.h>

namespace cbpp {
    template <typename key_t, typename value_t, size_t t_size> class StaticMap {
        struct Pair {
            key_t Key;
            value_t Value;
        };

        Pair m_aData[t_size];

        public:
            
    };
}

#endif
