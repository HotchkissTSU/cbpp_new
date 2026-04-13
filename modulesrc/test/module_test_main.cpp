#include <stdio.h>
#include <string.h>

#include "cbpp/Filesystem.h"

extern "C" int cbpp_main(int argc, char** argv) {
    printf("Module EP!\n");

    char sBuffer[512];
    char sBuffer2[512];

    while(1) {
        putc('>', stdout);
        scanf("%s", sBuffer);
        printf("%s\n", cbpp::ValidatePath(sBuffer, sBuffer2, 512));
    }

    return 0;
}
