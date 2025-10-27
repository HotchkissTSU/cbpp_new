#include "cbpp/error.h"

using namespace cbpp;

GErrorStack g_ErrorStack;

void Throw(const char* sMsg) {
    /*
        Unwind the level 1 errors stack
        Create a log dump
        Terminate the engine
    */

    
}
