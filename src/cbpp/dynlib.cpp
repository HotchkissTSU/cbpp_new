#include "cbpp/dynlib.h"

namespace cbpp {
    DynamicLibrary::DynamicLibrary(const char* sFilePath) {
        CBPP_ON_LINUX(
            m_hLibrary = dlopen(sFilePath, CBPP_DYNLIB_PARAMS);
            if(m_hLibrary == NULL) {
                // Do something about this!
            }
        )

        CBPP_ON_WINDOWS(
            CBPP_NO_WINDOWS_ASSERT
        )
    }

    void* DynamicLibrary::GetSymbol(const char* sName) {
        CBPP_ON_LINUX(
            dlerror();
            void* pSym = dlsym(m_hLibrary, sName);
            char* sErr = dlerror();
            if(sErr == NULL) {
                return pSym;
            }else{
                // Do something about this!
                return NULL;
            }
        )

        CBPP_ON_WINDOWS(
            CBPP_NO_WINDOWS_ASSERT
        )
    }

    DynamicLibrary::~DynamicLibrary() {
        CBPP_ON_LINUX(
            if(m_hLibrary != NULL) {
                dlclose(m_hLibrary);
            }
        )

        CBPP_ON_WINDOWS(
            CBPP_NO_WINDOWS_ASSERT
        )
    }
}
