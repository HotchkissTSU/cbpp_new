#ifndef CBPP_NETWORK_H
#define CBPP_NETWORK_H

#include "cbpp_api/Array.h"

/*
    We are building a serverside library by default
*/

#ifdef CBPP_CLIENTSIDE
    #define CBPP_ON_SERVER(code)
    #define CBPP_ON_CLIENT(code) code
#else
    #define CBPP_SERVERSIDE
    #define CBPP_ON_SERVER(code) code
    #define CBPP_ON_CLIENT(code)
#endif

#endif

#define CBPP_DEFAULT_PORT 32512

namespace cbpp {
    // Network package ID
    typedef uint32_t npid_t;    

    struct NetPackageReg {
        
    };

    class CNetPackageRegisty {
        friend CNetPackageRegisty* GetNetPackageRegistry();

        CNetPackageRegisty();

        public:
            CNetPackageRegisty(const CNetPackageRegisty& Other) = delete;
            CNetPackageRegisty& operator=(const CNetPackageRegisty& Other) = delete;
    };

    CNetPackageRegisty* GetNetPackageRegistry();
}

#endif
