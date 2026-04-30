#ifndef CBPP_DATAFILES_OBJMODEL_H
#define CBPP_DATAFILES_OBJMODEL_H

/*
    Object model interface, used both by CML and CDF
*/

#include <stdint.h>
#include <stddef.h>

#include "cbpp/Constants.h"

namespace cbpp::cdf {
    // Integer type to use
    typedef int32_t int_t;

    // Floating-point type to use
    typedef float float_t;

    enum class EObjectClass {
        Object,
        Array,
        Binary,
        Integer,
        Float,
        String
    };

    const char* ClassString(EObjectClass);

    enum class EPathError : uint32_t {
        Ok,                     // OK

        NotFound,               // Subobject does not exist
        ObjIndex,               // Indexing an object
        ArrayAccess,            // Accessing an array with '.'
        BadIndex,               // NaN or float is used as index
        BadSeparator            // Something except '.' is used as a separator
    };

    class IObject;

    class CObject {
        non_thread_safe static EPathError s_iPathError;

        IObject* m_pObj = NULL;

        public:
            CObject() = default;

            CObject(int_t iValue);
            CObject(float_t fValue);
            CObject(const char* sValue);

            CObject(IObject* pData);
            operator IObject*() const;
            IObject* GetPointer() const;

            EObjectClass Class() const;
            size_t Length() const;

            bool operator==(const CObject& pOther) const;
            bool operator!=(const CObject& pOther) const;

            CObject& operator=(int_t iValue);
            CObject& operator=(float_t fValue);
            CObject& operator=(const char* sValue);
            void SetBinaryData(const uint8_t* pData, size_t iLength);

            CObject operator[](size_t iIndex);
            CObject operator[](const char* sName);
            CObject Access(const char* sPath);

            const char* IndexName(size_t iIndex) const;

            void Push(CObject pObj);
            void Push(const char* sName, CObject pObj);

            explicit operator int_t() const;
            explicit operator float_t() const;
            explicit operator const char*() const;
            explicit operator uint8_t*();

            int_t AsInt() const;
            float_t AsFloat() const;
            const char* AsString() const;
            uint8_t* AsBinary();

            operator bool() const;
    };

    EPathError GetPathAccessError();

    void PrintObject(CObject pObj, size_t iDepth = 0);

    // Null value to signal errors
    extern const CObject NIL;

    CObject CreateObject(EObjectClass iClass);
    void DeleteObject(CObject pObj);
    CObject CopyObject(CObject pObj);
}

#endif
