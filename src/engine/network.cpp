#include "cbpp/Network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

namespace cbpp {
    void CSocket::Construct(int iType) {
        CBPP_ON_LINUX (
            m_iSocket = socket(AF_INET, iType, 0);
            CbAssertf(m_iSocket < 0, "Failed opening a socket: %s", strerror(errno));
        )

        CBPP_ON_WINDOWS (
            CBPP_NO_WINDOWS_ASSERT
        )
    }

    CSocket::CSocket(int iType) {
        this->Construct(iType);
    }

    CSocket::CSocket() {
        this->Construct(SOCK_DGRAM);
    }

    bool CSocket::IsValid() const {
        CBPP_ON_LINUX (
            return m_iSocket >= 0;
        )

        CBPP_ON_WINDOWS (
            CBPP_NO_WINDOWS_ASSERT
        )
    }

    bool CSocket::MakeNonblock() {
        int iFlags = fcntl(m_iSocket, F_GETFL, 0);
        if (fcntl(m_iSocket, F_SETFL, iFlags | O_NONBLOCK) == -1) {
            CbAssertf(true, "Failed to set socket to non-blocking mode: %s", strerror(errno));
            return false;
        }

        return true;
    }

    bool CSocket::SetBufferSize(size_t iSize) {
        int iRecvBuffLen = iSize;
        if (setsockopt(m_iSocket, SOL_SOCKET, SO_RCVBUF, &iRecvBuffLen, sizeof(iRecvBuffLen)) < 0) {
            CbAssertf(true, "Failed to set socket buffer size to %i: %s", iSize, strerror(errno));
            return false;
        }

        return true;
    }
    
    bool CSocket::Bind(NetAddress Addr) {
        struct sockaddr_in Address;
        Address.sin_family = AF_INET;
        Address.sin_port = htons(Addr.Port);
        Address.sin_addr.s_addr = Addr.Addr;

        if (bind(m_iSocket, (struct sockaddr *)(&Address), sizeof(Address)) < 0) {
            WriteLogf(ELogLevel::Error, "Failed binding a socket to the address %u.%u.%u.%u:%u : %s", 
                        Addr.AddrBytes[0], Addr.AddrBytes[1], Addr.AddrBytes[2], Addr.AddrBytes[3], Addr.Port, strerror(errno));
            
            return false;
        }

        return true;
    }

    CSocket::~CSocket() {
        CBPP_ON_LINUX (
            if(m_iSocket >= 0) {
                close(m_iSocket);
            }
        )

        CBPP_ON_WINDOWS (
            CBPP_NO_WINDOWS_ASSERT
        )
    }
}
