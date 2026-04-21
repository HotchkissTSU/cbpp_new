#include <stdio.h>
#include <string.h>

#include "cbpp/CML.h"

extern "C" int cbpp_main(int argc, char** argv) {
    printf("Module EP!\n");

    using namespace cbpp;

    char sBuffer[512];

    cml::CParser test;

    cml::EErrorType iCode = test.Parse("assets/test.cml");

    if( iCode != cml::EErrorType::Ok ) {
        test.FormatError(iCode, sBuffer, sizeof(sBuffer));
        puts(sBuffer);

        return -1;
    }

    while(true) {
        printf("> ");
        scanf("%s", sBuffer);

        cml::CObject pTest = test[sBuffer];

        if(pTest) {
            cml::PrintObject( pTest );
        } else {
            test.FormatError( test.GetPathError(), sBuffer, sizeof(sBuffer) );
            puts(sBuffer);
        }
    }

    return 0;
}
