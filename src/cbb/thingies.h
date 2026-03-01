#ifndef CBB_STATE_H
#define CBB_STATE_H

#include "cbpp/Array.h"
#include "target.h"

extern struct State {
    char* sFile = "cbb.json";
    int iLogLevel = 1;
    bool bIgnoreCache = false, bCleanup = false;

    cbpp::CArray<char*> aTargetsList;
    cbpp::CArray<BuildTarget> aFinalTargetsList;
} g_State;

enum ELogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,

    LOG_SILENT
};

void Print(ELogLevel iLvl, const char* sFmt, ...);

#endif
