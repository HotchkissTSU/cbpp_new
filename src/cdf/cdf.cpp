#include "cdf/cdf.h"

#include <string.h>

#include "cbpp/malloc_wrapper.h"

namespace cbpp::cdf {
    const char* ParsedPath::operator[](size_t iIndex) {
        return (const char*)(aSeparators[iIndex]);
    }

    ParsedPath ParseCDFPath(const char* sPath) {
        ParsedPath Out;

        const size_t iSourceLength = strlen(sPath);

        Out.sSourceString = Malloc<char>(iSourceLength+1);
        memcpy(Out.sSourceString, sPath, iSourceLength+1);

        size_t iCounter = 0;
        for(size_t i = 0; i < iSourceLength; i++) {
            if(Out.sSourceString[i] == '.' && (i < iSourceLength)) {
                Out.sSourceString[i] = '\0';

                Out.aSeparators = Realloc<char*>(Out.aSeparators, iCounter+1);
                
                Out.aSeparators[iCounter] = &Out.sSourceString[i+1];
                iCounter++;
            }
        }

        return Out;
    }
}
