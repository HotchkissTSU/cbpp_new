#ifndef CBPP_API_PRINT_H
#define CBPP_API_PRINT_H

#include <utility>

#include <stdio.h>

namespace cbpp {
    void Print(FILE* hStream, const char* sValue);

    template <typename... args_t> void Printv(FILE* hStream, args_t&&... Args) {
        
    }
}

#endif
