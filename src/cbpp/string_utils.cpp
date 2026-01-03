#include "cbpp/string_utils.h"

#include <string.h>

#include "cbpp/malloc_wrapper.h"

namespace cbpp {
    char* StringDup(const char* sSource, size_t* pLength) {
        const size_t iLength = strlen(sSource);
        char* pNew = Malloc<char>(iLength+1);
        memcpy(pNew, sSource, iLength+1);

        if(pLength != NULL) { *pLength = iLength; }

        return pNew;
    }

    size_t GetUnicodeSeqLength(char cChar) {
        if ((cChar & 0x80) == 0x00) {           // 0xxxxxxx
            return 1;
        } else if ((cChar& 0xE0) == 0xC0) {     // 110xxxxx
            return 2;
        } else if ((cChar & 0xF0) == 0xE0) {    // 1110xxxx
            return 3;
        } else if ((cChar & 0xF8) == 0xF0) {    // 11110xxx
            return 4;
        }
        return 1;
    }

    bool IsInsideUnicodeSeq(char cChar) {
        return (cChar & 0xC0) == 0x80; 
    }
}
