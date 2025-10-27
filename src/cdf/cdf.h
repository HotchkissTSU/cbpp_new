#ifndef CBPP_CDF_MAIN_H
#define CBPP_CDF_MAIN_H

#include <stddef.h>

namespace cbpp::cdf {
    class CParsedPath {
        char* m_sSourceString = NULL;
        char** m_aSeparators = NULL;

        public:
            CParsedPath() = delete;
            CParsedPath(const char* sSource);
            CParsedPath(const CParsedPath& Other) = delete;

            const char* operator[](size_t iIndex);
            CParsedPath& operator=(CParsedPath& Other);

            ~CParsedPath();
    };

    CParsedPath ParseCDFPath(const char* sPath);

    class CObject {
        
    };
}

#endif
