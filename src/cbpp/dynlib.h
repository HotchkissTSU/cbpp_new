#ifndef CBPP_DYNAMIC_LIB_H
#define CBPP_DYNAMIC_LIB_H

#include "cbpp/cbdef.h"

#ifdef CBPP_LINUX
    #include <dlfcn.h>
#else
    #include <windows.h>
#endif

#define CBPP_DYNLIB_PARAMS RTLD_NOW

namespace cbpp {
    class DynamicLibrary {
        CBPP_ON_LINUX(
            void* m_hLibrary = NULL;
        )

        public:
            DynamicLibrary(const DynamicLibrary& Other) = delete;
            DynamicLibrary() = delete;
            void operator=(DynamicLibrary& Other) = delete;

            DynamicLibrary(const char* sFilePath);

            void* GetSymbol(const char* sName);

            ~DynamicLibrary();
    };
}

#endif
