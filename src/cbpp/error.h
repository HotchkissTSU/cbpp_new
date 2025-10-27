#ifndef CBPP_ERROR_H
#define CBPP_ERROR_H

#include <stdio.h>

#include "cbpp/cbdef.h"

#define CBPP_T1ERR_STACK_SIZE 128

namespace cbpp {
    enum EErrorCode {
        Error_IO
    };

    struct ErrorTier1 {
        EErrorCode m_iCode;
        char* m_sErrorMsg = NULL;
    };

    extern struct GErrorStack {
        uint32_t m_iLength = 0;
        ErrorTier1 m_aStack[CBPP_T1ERR_STACK_SIZE];
    } g_ErrorStack;

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
