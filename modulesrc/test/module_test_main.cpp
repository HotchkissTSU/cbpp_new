#include <stdio.h>
#include <string.h>

#include "cbpp/CML.h"

extern "C" int cbpp_main(int argc, char** argv) {
    printf("Module EP!\n");

    using namespace cbpp;

    cml::CParser test;

    cml::EErrorType iCode = test.Parse("assets/test.cml");
    puts(cml::StringError(iCode));

    cml::PrintObject(test.Root());

    return 0;
}
