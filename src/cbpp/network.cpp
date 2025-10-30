#include "cbpp/network.h"

#include <stdio.h>

namespace cbpp::net {
    CPackageRegistrator::CPackageRegistrator(recv_callback_t fpCallback, const char* sPkgClass) {
        printf("Registrator call for '%s', function 0x%x\n", sPkgClass, fpCallback);
    }
}
