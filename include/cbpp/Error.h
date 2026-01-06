#ifndef CBPP_ERROR_H
#define CBPP_ERROR_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

// if expr == true -> terminating
#define CbAssert(expr, msg) if((bool)(expr) == true) {\
    printf("File: '%s', Line: %lu\nAssertion [ %s ] has failed: %s\n", strrchr(__FILE__, '/')+1, __LINE__, #expr, msg); exit(EXIT_FAILURE); }

// if expr == true -> terminating
#define CbAssertf(expr, msg, ...) if((bool)(expr) == true) {\
    char __buff[256]; snprintf(__buff, sizeof(__buff), msg, __VA_ARGS__);\
    printf("File: '%s', Line: %lu\nAssertion [ %s ] has failed: %s\n", strrchr(__FILE__, '/')+1, __LINE__, #expr, __buff); exit(EXIT_FAILURE); }

// Currently, i only have a linux computer, so some W*ndows-dependent code is yet impossible to write and test
#define CBPP_NO_WINDOWS_ASSERT static_assert(false, "Non-implemented Windows-dependent code here");

namespace cbpp {
    enum class EErrorCode : uint32_t {
        IO                              // Input/Output error
    };

    /*
        Lethal throws
    */
    void Throw(const char* sErrorText);
    void Throwf(const char* sFormat, ...);
    void Throwv(const char* sFormat, va_list vaList);

    /*
        Non-lethal throws
    */
    void PushError(EErrorCode iCode, const char* sMsg);
    void PushErrorf(EErrorCode iCode, const char* sFormat, ...);
    void PushErrorv(EErrorCode iCode, const char* sFormat, va_list vaList);
}

#endif
