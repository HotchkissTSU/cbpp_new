#ifndef CBPP_ERROR_H
#define CBPP_ERROR_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

//#define CBPP_DEBUG

#ifdef CBPP_DEBUG
    // if expr == true -> terminating
    #define CbAssert(expr, msg) if((bool)(expr) == true) {\
        printf("File: '%s', Line: %lu\nAssertion [ %s ] has failed: %s\n", strrchr(__FILE__, '/')+1, __LINE__, #expr, msg); exit(EXIT_FAILURE); }

    // if expr == true -> terminating
    #define CbAssertf(expr, msg, ...) if((bool)(expr) == true) {\
        char __buff[256]; snprintf(__buff, sizeof(__buff), msg, __VA_ARGS__);\
        printf("File: '%s', Line: %lu\nAssertion [ %s ] has failed: %s\n", strrchr(__FILE__, '/')+1, __LINE__, #expr, __buff); exit(EXIT_FAILURE); }
#else
    // if expr == true -> log error
    #define CbAssert(expr, msg) if((bool)(expr) == true) {\
        cbpp::GetGlobalLog()->Logf(cbpp::ELogLevel::Error, "File: '%s', Line: %lu; Assertion [ %s ] has failed: %s", \
        strrchr(__FILE__, '/')+1, __LINE__, #expr, msg); }

    // if expr == true -> log error
    #define CbAssertf(expr, msg, ...) if((bool)(expr) == true) {\
        char __buff[256]; snprintf(__buff, sizeof(__buff), msg, __VA_ARGS__); \
        cbpp::GetGlobalLog()->Logf(cbpp::ELogLevel::Error, "File: '%s', Line: %lu; Assertion [ %s ] has failed: %s", \
        strrchr(__FILE__, '/')+1, __LINE__, #expr, __buff); }
#endif

// Currently, i only have a linux computer, so some W*ndows-dependent code is yet impossible to write and test
#define CBPP_NO_WINDOWS_ASSERT static_assert(false, "Non-implemented W*ndows-dependent code here");

namespace cbpp {
    enum class ELogLevel : uint32_t {
        Debug,
        Info,
        Warning,
        Error,

        Nothing
    };

    /*
        Lethal throws
    */
    void Throwf(const char* sFormat, ...);
    void Throwv(const char* sFormat, va_list vaList);

    /*
        Direct logging to the global log
    */
    void WriteLogf(ELogLevel iLevel, const char* sFormat, ...);
    void WriteLogv(ELogLevel iLevel, const char* sFormat, va_list vaList);

    class CLogger {
        FILE* m_hFile = NULL;
        const char* m_sName;

        #ifdef CBPP_DEBUG
            ELogLevel m_iLevel = ELogLevel::Debug;
        #else
            ELogLevel m_iLevel = ELogLevel::Info;
        #endif

        public:
            CLogger() = delete;
            CLogger(const CLogger&) = delete;
            CLogger(CLogger&&) = delete;

            CLogger& operator=(const CLogger&) = delete;
            CLogger& operator=(CLogger&&) = delete;

            void SetLoggingLevel(ELogLevel iMinLevel);

            void Logf(ELogLevel iLevel, const char* sFormat, ...);
            void Logv(ELogLevel iLevel, const char* sFormat, va_list vaList);

            CLogger(const char* sLogName);
            ~CLogger() = default;
    };

    CLogger* GetGlobalLog();
}

#endif
