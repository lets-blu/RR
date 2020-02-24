#ifndef __ETHERNET_H__
#define __ETHERNET_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "string.h"

#include "keywords.h"

#define ETHERNET_IP_LENGTH  4
#define ETHERNET_MAC_LENGTH 6

struct Ethernet {
    bool _dhcpEnabled;
    uint8_t * _dhcpBuffer;
    
    // local parameters
    uint16_t _localPort;
    uint8_t _localIP[ETHERNET_IP_LENGTH];
    
    // server parameters
    uint16_t _serverPort;
    uint8_t _serverIP[ETHERNET_IP_LENGTH];
    
    // network parameters
    uint8_t _subnetMask[ETHERNET_IP_LENGTH];
    uint8_t _defaultGateway[ETHERNET_IP_LENGTH];

    // rx buffer
    uint8_t * _rxBuffer;
    uint8_t _rxBufferSize, _rxDataCount;

    // tx buffer
    uint8_t * _txBuffer;
    uint8_t _txBufferSize, _txDataCount;

    // virtual method(s)
    void (*onReceive)(struct Ethernet * pThis);
    uint8_t (*send)(struct Ethernet * pThis, uint8_t * buffer, uint8_t length);
};

typedef void (*on_ethernet_receive_fp)(struct Ethernet *);
typedef uint8_t (*ethernet_send_fp)(struct Ethernet *, uint8_t *, uint8_t);

// Abstruct class constructor
PUBLIC struct Ethernet newEthernet(void);

// Public method(s)
PUBLIC void setEthernetDHCPBuffer(struct Ethernet * pThis, uint8_t * buffer);
PUBLIC void setEthernetLocalPort(struct Ethernet * pThis, uint16_t localPort);
PUBLIC void setEthernetDHCPEnabled(struct Ethernet * pThis, bool enabled);

PUBLIC void setEthernetLocalParameters(struct Ethernet * pThis, 
    uint8_t * localIP, uint16_t localPort);
PUBLIC void setEthernetServerParameters(struct Ethernet * pThis, 
    uint8_t * serverIP, uint16_t serverPort);
PUBLIC void setEthernetNetworkParameters(struct Ethernet * pThis, 
    uint8_t * subnetMask, uint8_t * defaultGateway);

PUBLIC void setEthernetRxBuffer(struct Ethernet * pThis, 
    uint8_t * rxBuffer, uint8_t rxBufferSize);
PUBLIC void setEthernetTxBuffer(struct Ethernet * pThis, 
    uint8_t * txBuffer, uint8_t txBufferSize);

PUBLIC VIRTUAL void defaultOnEthernetReceive(struct Ethernet * pThis);
PUBLIC VIRTUAL uint8_t defaultEthernetSend(struct Ethernet * pThis, 
    uint8_t * buffer, uint8_t length);

PUBLIC STATIC uint8_t * getEthernetLocalMAC(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ETHERNET_H__
