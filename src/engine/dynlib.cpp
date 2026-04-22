#include "cbpp/Dynlib.h"

#include "cbpp/Error.h"
#include <errno.h>

namespace cbpp {
    CDynamicLibrary::CDynamicLibrary(const char* sFilePath) : m_sLibName(sFilePath) {
        CBPP_ON_LINUX (
            m_hLibrary = dlopen(sFilePath, CBPP_DYNLIB_PARAMS);
            if(m_hLibrary == NULL) {
                Throwf("Unable to open '%s' as a shared library (%s)", sFilePath, strerror(errno));
            }
        )

        CBPP_ON_WINDOWS (
            CBPP_NO_WINDOWS_ASSERT
        )
    }

    void* CDynamicLibrary::GetSymbol(const char* sName) {
        CBPP_ON_LINUX (
            dlerror();
            void* pSym = dlsym(m_hLibrary, sName);
            char* sErr = dlerror();
            if(sErr == NULL) {
                return pSym;
            }else{
                WriteLogf(ELogLevel::Error, "Failed to retreive the '%s' symbol from '%s'", sName, m_sLibName);
                return NULL;
            }
        )

        CBPP_ON_WINDOWS (
            CBPP_NO_WINDOWS_ASSERT
        )
    }

    CDynamicLibrary::~CDynamicLibrary() {
        CBPP_ON_LINUX (
            if(m_hLibrary != NULL) {
                dlclose(m_hLibrary);
            }
        )

        CBPP_ON_WINDOWS (
            CBPP_NO_WINDOWS_ASSERT
        )
    }
}
