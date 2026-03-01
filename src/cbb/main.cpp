#include <stdio.h>

#include "yyjson.h"
#include "thingies.h"

State g_State;

void ParseCMD(int argc, char** argv);
void Prepare();
void Cleanup();

int main(int argc, char** argv) {
    ParseCMD(argc, argv);
    g_State.aTargetsList.Shrink();

    Prepare();

    Print(LOG_DEBUG, "Targets list:\n");
    for(int i = 0; i < g_State.aTargetsList.Length(); i++) {
        Print(LOG_DEBUG, "\t%s\n", g_State.aTargetsList[i]);
    }

    Print(LOG_INFO, "Project file: '%s'\n", g_State.sFile);

    if(g_State.bCleanup == true) {
        Print(LOG_INFO, "Removing cache and object files");
        Cleanup();
    }
}
