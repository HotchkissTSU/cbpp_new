#ifndef CBPP_NETWORK_H
#define CBPP_NETWORK_H

#include <netinet/in.h>

#include "cbpp/Constants.h"
#include "cbpp/Array.h"

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

#define CBPP_DEFAULT_PORT 32512

namespace cbpp {

    struct NetAddress {
        union {
            char AddrBytes[4];
            uint32_t Addr;
        };

        uint16_t Port = CBPP_DEFAULT_PORT;
    };

    /*
        1) Create a socket
        2) Setup it
        3) Bind it to an address
        4) Start doing thingies
    */

    class CSocket {
        private:
            void Construct(int iType);

            CBPP_ON_LINUX (
                int m_iSocket;
            )

        public:
            // Defaults to SOCK_DGRAM (UDP)
            CSocket();

            /*
                SOCK_DGRAM for UDP
                SOCK_STREAM for TCP
            */
            CSocket(int iType);

            CSocket(const CSocket&) = delete;
            CSocket(CSocket&&) = delete;
            CSocket& operator=(const CSocket&) = delete;

            bool IsValid() const;

            bool SetFlags(int iFlags);

            // Set this socket to non-blocking mode
            bool MakeNonblock();

            // Set the internal data buffer size
            bool SetBufferSize(size_t iSize);

            bool Bind(NetAddress Addr);

            ~CSocket();
    };
}

#endif
