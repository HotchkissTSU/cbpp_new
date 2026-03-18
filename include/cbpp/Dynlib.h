#ifndef CBPP_DYNAMIC_LIB_H
#define CBPP_DYNAMIC_LIB_H

#include <stddef.h>

#include "cbpp/Constants.h"

#ifdef CBPP_LINUX
    #include <dlfcn.h>
#else
    #include <windows.h>
#endif

#define CBPP_DYNLIB_PARAMS RTLD_NOW

namespace cbpp {
    class CDynamicLibrary {
        const char* m_sLibName;

        CBPP_ON_LINUX (
            void* m_hLibrary = NULL;
        )

        public:
            CBPP_PROTECTED_CLASS(CDynamicLibrary);

            CDynamicLibrary(const char* sFilePath);

            void* GetSymbol(const char* sName);

            ~CDynamicLibrary();
    };
}

#endif
