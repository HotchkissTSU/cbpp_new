#include "cbpp/Error.h"

#include <stdarg.h>
#include <stdlib.h>

#include "cbpp/String.h"

namespace cbpp {
    const char* GetLogLevelString(ELogLevel iLevel) {
        switch(iLevel) {
            case ELogLevel::Debug:      return "DEBUG";
            case ELogLevel::Info:       return "INFO ";
            case ELogLevel::Warning:    return "WARN ";
            case ELogLevel::Error:      return "ERROR";
            case ELogLevel::Nothing:    return " <!> ";
        }

        return ":)";
    }

    CLogger::CLogger(const char* sLogName) : m_sName(StringDup(sLogName)) {
        Logf(ELogLevel::Info, " == Log start == ");
    }

    void CLogger::Logv(ELogLevel iLevel, const char* sFormat, va_list Args) {
        static char s_sBuffer[1024];

        if((int)iLevel < (int)m_iLevel) { return; }

        m_hFile = fopen(m_sName, "ab");
        if(m_hFile == NULL) { return; }

        char sTimeBuff[128];
        StringFormatTime(sTimeBuff, sizeof(sTimeBuff), "[%H:%M:%S]");

        snprintf(s_sBuffer, sizeof(s_sBuffer), "%s %s: ", sTimeBuff, GetLogLevelString(iLevel));
        vsnprintf(s_sBuffer, sizeof(s_sBuffer), sFormat, Args);
        
        printf("%s\n", s_sBuffer);
        fprintf(m_hFile, "%s\n", s_sBuffer);

        fclose(m_hFile);
    }

    void CLogger::Logf(ELogLevel iLevel, const char* sFormat, ...) {
        va_list Args;
        va_start(Args, sFormat);

        this->Logv(iLevel, sFormat, Args);

        va_end(Args);
    }

    void CLogger::SetLoggingLevel(ELogLevel iLvl) { m_iLevel = iLvl; }

    CLogger* GetGlobalLog() {
        static CLogger s_hGlobalLogger("latest.log");
        return &s_hGlobalLogger;
    }
}

// Throws

namespace cbpp {
    void Throwv(const char* sFormat, va_list List) {
        CLogger* pLog = GetGlobalLog();
        pLog->Logv(ELogLevel::Error, sFormat, List);

        exit(EXIT_FAILURE);
    }

    void Throwf(const char* sFormat, ...) {
        va_list Args;
        va_start(Args, sFormat);

        Throwv(sFormat, Args);

        va_end(Args);
    }

    void WriteLogf(ELogLevel iLevel, const char* sFormat, ...) {
        va_list Args;
        va_start(Args, sFormat);
        GetGlobalLog()->Logv(iLevel, sFormat, Args);
        va_end(Args);
    }

    void WriteLogv(ELogLevel iLevel, const char* sFormat, va_list vaList) {
        GetGlobalLog()->Logv(iLevel, sFormat, vaList);
    }
}
