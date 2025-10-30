#ifndef CBPP_NETWORK_H
#define CBPP_NETWORK_H

#include "cbpp/cbdef.h"
#include "cbpp_api/Array.h"

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
