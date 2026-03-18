#ifndef CB_ENTRYP_MAIN_H
#define CB_ENTRYP_MAIN_H

#include <stddef.h>

extern struct cbsv_state_t {
    char* sGamefilePath = NULL;
} g_State;

void ParseArgs(int argc, char** argv);

#endif
