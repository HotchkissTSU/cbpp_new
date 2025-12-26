#include "cbpp/filesystem.h"

#include <string.h>
#include <stdio.h>

#include "cbpp/cbdef.h"
#include "cbpp/bit.h"
#include "cbpp_api/Math.h"

namespace cbpp {
    void ParsePath(const char* sPath) {
        EFileSystem iTargetFS;
        char sFSNameBuff[8];
        const char* pColon = strchr(sPath, ':');
        char* pCurrent;

        if(pColon == NULL) {    // No FS is specified, assume default
            iTargetFS = EFileSystem::Physical;
        }else {
            const ptrdiff_t iFSNameLen = pColon - sPath;
            memcpy(sFSNameBuff, sPath, Clamp(iFSNameLen, (ptrdiff_t)0, (ptrdiff_t)8));
            sFSNameBuff[iFSNameLen] = '\0';

            if( strcmp(sFSNameBuff, "CPK") == 0 ) {
                iTargetFS = EFileSystem::Virtual;
            } else if( strcmp(sFSNameBuff, "OS") == 0 ) {
                iTargetFS == EFileSystem::Physical;
            }
            printf("FS: %s\n", sFSNameBuff);
        }

        pCurrent = (char*)strchr(sPath, '/') + 1;
        
        while(pCurrent != NULL) {            
            const char* pBlockEnd = strchr(pCurrent+1, '/');
            if(pBlockEnd == NULL) {
                printf("%s\n", pCurrent);
                break;
            }

            for(char* pC = pCurrent; pC != pBlockEnd && *pC != '\0'; pC++) {
                putc(*pC, stdout);
            }
            putc('\n', stdout);

            pCurrent = (char*)pBlockEnd + 1;
        }
    }
}
