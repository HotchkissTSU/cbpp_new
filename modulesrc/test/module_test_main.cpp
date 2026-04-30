#include <stdio.h>
#include <string.h>

#include "cbpp/CML.h"
#include "cbpp/Benchmark.h"

extern "C" int cbpp_main(int argc, char** argv) {
    printf("Module EP!\n");

    using namespace cbpp;

    char sBuffer[512];

    cdf::CTextParser test;
    cdf::ETextError iCode = test.Parse("assets/test.cml");

    if( iCode != cdf::ETextError::Ok ) {
        test.FormatError(iCode, sBuffer, sizeof(sBuffer));
        puts(sBuffer);

        return -1;
    }

    cdf::PrintObject(test.Root());

    while(true) {
        printf("> ");
        scanf("%s", sBuffer);

        cdf::CObject pTest = test[sBuffer];

        if(pTest) {
            cdf::PrintObject( pTest );
        } else {
            continue;
        }
    }

    return 0;
}
