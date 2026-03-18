#ifndef CBPP_MODULE_H
#define CBPP_MODULE_H

#include <stddef.h>

/*
    See the example module for more details
*/

namespace cbpp {
    struct ModuleInfo {
        constexpr static const char* sGetInfoFuncName = "cbpp_info";

        typedef ModuleInfo* (*fpgetinfo_t)(void);     // Callback for gettong module info
        typedef int (*fpmain_t)(int, char**);         // Module-specific entry point

        // Callbacks

        fpmain_t fpMain = NULL;
    };
}

#endif
