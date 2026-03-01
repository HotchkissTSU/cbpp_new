/*
    Here are some rules about CB++ networking design:

    1) Server is always right

    2) Clientside entities shall only perform rendering, input handling and some other non-important code,
        unless the entity is completely clientside.

    3) Vice versa, serverside entities do all the work: listen for client`s inputs, do thingies and then send themselves to
        clients for them to copy and interp to.

    4) Entity IDs must be synced between all clients, if the entity is not completely clientside. Otherwise, such entities must not collide
        with other server-managed IDs. Server does all of the entity creation/deletion stuff and then tells everybody via the net messages

    5) Clients only receive updates on entities inside their PVS. Getting in/out of the PVS should not be interpolated. 

    6) Clientside / serverside class interfaces should be as similar as possible in the sake of readability. Functions, which are restricted
        for a specific side, should throw assertions.
*/

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
