#include "cbep/cbep.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "cbpp/Error.h"

void ParseArgs(int argc, char** argv) {
    for(int i = 1; i < argc; i++) {
        const char* sCurrent = argv[i] + 1;
        const char* sNext = (i != argc-1) ? argv[i+1] : NULL;

        if( (strcmp(sCurrent, "-game") == 0) || (strcmp(sCurrent, "g") == 0) ) { // Gamefile specified
            if(sNext == NULL) {
                cbpp::WriteLogf(cbpp::ELogLevel::Error, "[CBEP] cmd option '--game' (or '-g') requires an argument");
                continue;
            }

            g_State.sGamefilePath = (char*)sNext;

        } else if ( (strcmp(sCurrent, "-verbosity") == 0) || (strcmp(sCurrent, "v") == 0) ) { // Verbosity level specified
            int iLogLevel = 0;
            if(sNext == NULL) {
                cbpp::WriteLogf(cbpp::ELogLevel::Error, "[CBEP] cmd option '--verbosity' (or '-v') requires an argument");
            } else {
                iLogLevel = atoi(sNext);
                cbpp::GetGlobalLog()->SetLoggingLevel((cbpp::ELogLevel)( (int)cbpp::ELogLevel::Nothing - iLogLevel ));
            }
        }
    }
}
