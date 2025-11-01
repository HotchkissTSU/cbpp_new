#ifndef CBPP_ENV_VARIABLE_H
#define CBPP_ENV_VARIABLE_H

/*
    Environment variables

    These are used to track engine`s state. There are different namespaces for
    variables (engine / module / map / session / etc.)
*/

#include <stdint.h>

#include "cbpp/const_string.h"
#include "cbpp/mutable_string.h"
#include "cbpp_api/Array.h"

namespace cbpp {
    // Namespace for environment variables
    class CEVarScope {
        
    };

    // Environment variable type
    enum class EEVarType : uint32_t {
        Integer,
        Float,
        String
    };

    enum class EEVarFlags : uint32_t {
        Persist     =   1 << 0,             //  Save this variable between engine sessions
        NetSync     =   1 << 1,             //  Server will send this variable to clients
        Protected   =   1 << 2,             //  When NetSyncing, only server will be able to change this variable
        Admin       =   1 << 3,             //  When NetSyncing, only operators will be able to change this variable
    };

    // An environment variable
    class CEnvVar {
        union {
            int32_t i;
            float f;
            CString s;  // EnvVar string value can`t be NULL! We use "" instead
        } m_Data;
        
        EEVarType m_iType = EEVarType::Integer;

        public:
            CEnvVar() = delete;
            CEnvVar(const CEnvVar& Other) = delete;
            CEnvVar& operator=(const CEnvVar& Other) = delete;

            CEnvVar(int32_t iValue);
            CEnvVar(float fValue);
            CEnvVar(const char* sValue);

            EEVarType Type() const;

            int32_t GetInteger() const;
            float GetFloat() const;
            const char* GetString() const;

            bool SetValue(int32_t iValue);
            bool SetValue(float fValue);
            bool SetValue(const char* sValue);

            bool GetBool() const;

            ~CEnvVar();
    };
}

#endif
