#ifndef CBPP_NETWORK_H
#define CBPP_NETWORK_H

#include "enet/enet.h"

/*
    Register a callback for the specified network messages class

    Usage:

    CbNetMessage(my_pkg_class) { [callback code here] }
*/
#define CbNetMessage(pkg_class)\
    bool __NetPkgCallback_##pkg_class(void* pPackageData, size_t iPackageLength);\
    static cbpp::net::CPackageRegistrator __s_NetPkgCallbackRegistrator_##pkg_class(__NetPkgCallback_##pkg_class, #pkg_class);\
    bool __NetPkgCallback_##pkg_class(void* pPackageData, size_t iPackageLength)

namespace cbpp::net {
    // Network callback function
    typedef bool (*recv_callback_t)(void*, size_t);

    // A static registrator for our network callbacks
    class CPackageRegistrator {
        public:
            CPackageRegistrator(recv_callback_t fpCallback, const char* sPkgClass);
    };
}

#endif
