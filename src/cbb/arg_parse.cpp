#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "thingies.h"

void Print(ELogLevel iLvl, const char* sFmt, ...) {
    if((int)iLvl < (int)g_State.iLogLevel) { return; }

    va_list Args;
    va_start(Args, sFmt);
    vprintf(sFmt, Args);
    va_end(Args);
}

void Prepare() {
    system(__linux__ ? "mkdir .obj > /dev/null 2>&1" : "mkdir .obj");
    system(__linux__ ? "mkdir .cbb > /dev/null 2>&1" : "mkdir .cbb");
}

void Cleanup() {
    system("rm -r ./obj/* > /dev/null 2>&1");
}

const char* g_sHelpMessage = "CB++ Builder\n\
Tries to open 'cbb.json' by default\n\
Options:\n\
    -h          - Get this message and exit\n\
    -v [0-4]    - Set verbosity level (default is 1)\n\
    -f [path]   - Set project file name\n\
    -t [target] - Append a target to the queue\n\
    -c          - Clear cache and object files\n\
    -r          - Forcibly recompile everything";

void ParseCMD(int argc, char** argv) {
    for(int i = 1; i < argc; i++) {
        const char* sCurrent = argv[i] + 1;
        const char* sNext = (i != argc-1) ? argv[i+1] : NULL;

        if( strcmp(sCurrent, "h") == 0 ) {
            puts(g_sHelpMessage);
            exit(0);

        } else if( strcmp(sCurrent, "v") == 0 ) {
            if(sNext == NULL) {
                puts("-v expects an argument");
                exit(-1);
            }

            g_State.iLogLevel = (int)LOG_SILENT - atoi(sNext);

        } else if( strcmp(sCurrent, "f") == 0 ) {
            if(sNext == NULL) {
                puts("-f expects an argument");
                exit(-1);
            }

            g_State.sFile = (char*)sNext;

        } else if(strcmp(sCurrent, "t") == 0) {
            if(sNext == NULL) {
                puts("-t expects an argument");
                exit(-1);
            }

            g_State.aTargetsList.PushBack((char*)sNext);

        } else if(strcmp(sCurrent, "c") == 0) {
            puts("Removing object files");

            g_State.bCleanup = true;

        } else if(strcmp(sCurrent, "r") == 0) {
            g_State.bIgnoreCache = true;
        }
    }
}
