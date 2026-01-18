#ifndef CBPP_STRING_UTIL_H
#define CBPP_STRING_UTIL_H

#include <stddef.h>

namespace cbpp {
    /*
        Get length of the upcoming UTF-8 sequence.
        Returns 1 for ASCII codepoints
    */
    size_t GetUnicodeSeqLength(char cChar);

    /*
        Check whether this character is supposed to be inside UTF-8 sequence
    */
    bool IsInsideUnicodeSeq(char cChar);

    /*
        Like strcmp, but checks from the right to the left
    */
    int StringRightCmp(const char* sA, const char* sB);
}

#endif
