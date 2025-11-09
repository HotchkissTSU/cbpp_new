#include "cbpp/network.h"

#include <stdio.h>
#include <string.h>

#include "cbpp/cbdef.h"

namespace cbpp::net {
    void CMessage::Push16(uint16_t* pBytes) {
        uint16_t iNB = htons(*pBytes);
        m_aPayload.PushBytes(&iNB, sizeof(uint16_t));
    }

    void CMessage::Push32(uint32_t* pBytes) {
        uint32_t iNB = htonl(*pBytes);
        m_aPayload.PushBytes(&iNB, sizeof(uint32_t));
    }

    void CMessage::Push64(uint64_t* pBytes) {
        uint64_t iNB = htonll(*pBytes);
        m_aPayload.PushBytes(&iNB, sizeof(uint64_t));
    }

    byte_t ProcessIPByte(const char* sText) {
        int iNum = strtol(sText, (char**)NULL, 10);
        if(iNum > 255) {
            return 0;
        }else {
            return (byte_t)iNum;
        }
    }

    CIPAddress::CIPAddress(const char* sAddress) {
        const char* sPort = strrchr(sAddress, ':') + 1;
        bool bHasPort = (sPort-1 != NULL);

        if(bHasPort) {
            m_iPort = (uint16_t)strtol(sPort, (char**)NULL, 10);
        }else {
            m_iPort = CBPP_DEFAULT_PORT;
        }

        char sByteBuffer[4]; sByteBuffer[3] = '\0';
        char* pCurrent = (char*)sAddress;
        uint32_t iCounter = 0, iBytesCounter = 0;;
        
        forever {
            if(*pCurrent == '.' || *pCurrent == ':') {
                m_aAddr[iBytesCounter] = ProcessIPByte(sByteBuffer);
                memset(sByteBuffer, 0, sizeof(sByteBuffer));
                iBytesCounter++;
                iCounter = 0;
            } else {
                sByteBuffer[ iCounter % 3 ] = *pCurrent;
                iCounter++;
            }

            pCurrent++;
            if(*pCurrent == '\0' || pCurrent+1 == sPort) {
                m_aAddr[iBytesCounter] = ProcessIPByte(sByteBuffer);
                break;
            }
        }
    }

    void CIPAddress::Print() const {
        for(size_t i = 0; i < sizeof(m_aAddr); i++) {
            printf("%u", m_aAddr[i]);
            if(i != sizeof(m_aAddr) - 1) {
                printf("%c", '.');
            }
        }
        printf(":%u\n", m_iPort);
    }
}
