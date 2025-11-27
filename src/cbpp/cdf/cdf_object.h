#ifndef CBPP_CDF_OBJECT_H
#define CBPP_CDF_OBJECT_H

#include <stddef.h>
#include <stdint.h>

namespace cbpp::cdf {
    typedef int32_t int_t;

    enum class EObjectClass : int_t {

    };

    class IObject {
        public:
            virtual size_t Sizeof() const = 0;
            virtual size_t Length() const = 0;
            virtual EObjectClass Class() const = 0;
    };

    template <typename T> class CObjectStorage {
        private:
            T* m_pData = NULL;
            int_t m_iLength = 0;
            EObjectClass m_iClass;
    };
}

#endif
