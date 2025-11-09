#ifndef CBPP_NETWORK_H
#define CBPP_NETWORK_H

#include "cbpp/byte_array.h"

#if __BIG_ENDIAN__
    #define htonll(x) (x)
    #define ntohll(x) (x)
#else
    #define htonll(x) (((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
    #define ntohll(x) (((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))
#endif

#ifdef CBPP_LINUX
    #include <arpa/inet.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
#elif CBPP_WINDOWS
    CBPP_NO_WINDOWS_ASSERT
#endif

namespace cbpp::net {
    // Network callback function
    typedef bool (*recv_callback_t)(void*, size_t);

    enum EMessageType : uint16_t {
        MSG_INVALID,
        MSG_NET_TEST
    };

    // IPv4 address with port
    class CIPAddress {
        private:
            byte_t m_aAddr[4];
            uint16_t m_iPort;

        public:
            // xxx.xxx.xxx.xxx:yyyyy
            CIPAddress(const char* sAddress);

            void Print() const;
    };

    class INetSocket {
        public:
            INetSocket();
            virtual ~INetSocket() = 0;

            virtual bool Bind();
    };

    // OS-independent UDP socket
    class CUDPSocket {
        CBPP_ON_LINUX(
            int m_hSocket;
        )

        CBPP_ON_WINDOWS(
            CBPP_NO_WINDOWS_ASSERT
        )
    };

    class CMessage {
        CByteArray m_aPayload;
        EMessageType m_iClassID = MSG_INVALID;
        size_t m_iCursor = 0;   // A cursor for consecutive reads

        public:
            CMessage(EMessageType iType);

            /*
                PUSHERS
                Write some bytes into the message using the network byte order
            */
            void Push16(uint16_t* pBytes);
            void Push32(uint32_t* pBytes);
            void Push64(uint64_t* pBytes);

            /*
                GETTERS
                Allow for consecutive data extraction, represented in the host byte order
                Return false upon hitting the payload`s end
            */
            bool Recv16(uint16_t* pTarget);
            bool Recv32(uint32_t* pTarget);
            bool Recv64(uint64_t* pTarget);

            // Reset the reading cursor position
            void ResetCursor();

            // Length of the payload
            size_t Length() const;
    };
}

#endif
