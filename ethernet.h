#ifndef __ETHERNET_H__
#define __ETHERNET_H__

#include "stdbool.h"
#include "stdlib.h"
#include "stm32f10x.h"
#include "string.h"

#include "keywords.h"

#define ETHERNET_IP_LEN     ((uint8_t)4)
#define ETHERNET_MAC_LEN    ((uint8_t)6)
#define ETHERNET_CLIENT_MAC ((uint8_t []){0x00, 0x08, 0xdc, 0x00, 0xab, 0xcd})

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    struct Ethernet {
        uint16_t	_serverPort;
        uint8_t		_serverIP[ETHERNET_IP_LEN];

        uint16_t	_clientPort;
        uint8_t		_clientIP[ETHERNET_IP_LEN];

        uint8_t		_subnetMask[ETHERNET_IP_LEN];
        uint8_t		_gatewayIP[ETHERNET_IP_LEN];

        uint8_t *	_rxBuffer;
        uint8_t		_rxBufferSize, _numOfRxData;

        uint8_t *	_txBuffer;
        uint8_t		_txBufferSize, _numOfTxData;

        bool(*init)(struct Ethernet * pThis);
        uint8_t(*send)(struct Ethernet * pThis, uint8_t message[], uint8_t length);
        void(*onReceive)(struct Ethernet * pThis);
    };

    // abstract class (de)constructor(s)
    PUBLIC struct Ethernet newEthernet(void);

    // public method(s)
    PUBLIC void setEthernetServerParams(struct Ethernet * pThis,
        uint8_t serverIP[], uint16_t serverPort);
    PUBLIC void setEthernetClientParams(struct Ethernet * pThis,
        uint8_t clientIP[], uint16_t clientPort);
    PUBLIC void setEthernetNetworkParams(struct Ethernet * pThis,
        uint8_t subnetMask[], uint8_t gatewayIP[]);
    PUBLIC void setEthernetRxBuffer(struct Ethernet * pThis,
        uint8_t * rxBuffer, uint8_t rxBufferSize);
    PUBLIC void setEthernetTxBuffer(struct Ethernet * pThis,
        uint8_t * txBuffer, uint8_t txBufferSize);
    PUBLIC VIRTUAL bool defaultEthernetInit(struct Ethernet * pThis);
    PUBLIC VIRTUAL uint8_t defaultEthernetSend(struct Ethernet * pThis,
        uint8_t message[], uint8_t length);
    PUBLIC VIRTUAL void defaultOnEthernetReceive(struct Ethernet * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ETHERNET_H__
