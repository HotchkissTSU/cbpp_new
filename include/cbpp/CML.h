#ifndef CBPP_CML_API_H
#define CBPP_CML_API_H

#include <stdint.h>
#include <stddef.h>

namespace cbpp::cml {
    typedef int32_t int_t;
    typedef float float_t;

    enum class EObjectClass {
        Object,
        Array,
        Binary,
        Integer,
        Float,
        String
    };

    class IObject {
        public:
            virtual IObject* At(const char*) = 0;
            virtual IObject* At(size_t) = 0;

            virtual EObjectClass Class() const = 0;

            virtual int_t AsInt() const = 0;
            virtual float_t AsFloat() const = 0;
            virtual const char* AsString() const = 0;

            virtual void SetValue(int_t) = 0;
            virtual void SetValue(float_t) = 0;
            virtual void SetValue(const char*) = 0;

            virtual size_t Length() const = 0;

            virtual ~IObject() = 0;
    };

    IObject* CreateObject(EObjectClass iClass);
}

#endif
