#include "ethernet.h"

#define IS_ETHERNET_BUFFER_VALID(buffer, length) \
    ((buffer) != NULL && (length) > 0)

#define IS_ETHERNET_TX_BUFFER_EMPTY(pThis) \
    ((pThis)->_txBuffer != NULL && (pThis)->_txDataCount == 0)

PRIVATE STATIC uint8_t * localMAC;

PUBLIC struct Ethernet newEthernet(void)
{
    struct Ethernet ethernet = {
        ._dhcpEnabled       = false,
        ._dhcpBuffer        = NULL,
        
        ._localPort         = 0, 
        ._localIP           = {0},
        
        ._serverPort        = 0,
        ._serverIP          = {0}, 
        
        ._subnetMask        = {0}, 
        ._defaultGateway    = {0}, 

        ._rxBuffer          = NULL, 
        ._rxBufferSize      = 0, 
        ._rxDataCount       = 0, 

        ._txBuffer          = NULL, 
        ._txBufferSize      = 0, 
        ._txDataCount       = 0, 

        .onReceive          = defaultOnEthernetReceive, 
        .send               = defaultEthernetSend
    };

    return ethernet;
}

PUBLIC void setEthernetDHCPBuffer(struct Ethernet * pThis, uint8_t * buffer)
{
    if (buffer != NULL)
    {
        pThis->_dhcpBuffer = buffer;
    }
}

PUBLIC void setEthernetLocalPort(struct Ethernet * pThis, uint16_t localPort)
{
    pThis->_localPort = localPort;
}

PUBLIC void setEthernetDHCPEnabled(struct Ethernet * pThis, bool enabled)
{
    pThis->_dhcpEnabled = enabled;
}

PUBLIC void setEthernetLocalParameters(struct Ethernet * pThis, 
    uint8_t * localIP, uint16_t localPort)
{
    if (localIP != NULL)
    {
        pThis->_localPort = localPort;
        memcpy(pThis->_localIP, localIP, ETHERNET_IP_LENGTH);
    }
}

PUBLIC void setEthernetServerParameters(struct Ethernet * pThis, 
    uint8_t * serverIP, uint16_t serverPort)
{
    if (serverIP != NULL)
    {
        pThis->_serverPort = serverPort;
        memcpy(pThis->_serverIP, serverIP, ETHERNET_IP_LENGTH);
    }
}

PUBLIC void setEthernetNetworkParameters(struct Ethernet * pThis, 
    uint8_t * subnetMask, uint8_t * defaultGateway)
{
    if (subnetMask != NULL && defaultGateway != NULL)
    {
        memcpy(pThis->_subnetMask, subnetMask, ETHERNET_IP_LENGTH);
        memcpy(pThis->_defaultGateway, defaultGateway, ETHERNET_IP_LENGTH);
    }
}

PUBLIC void setEthernetRxBuffer(struct Ethernet * pThis, 
    uint8_t * rxBuffer, uint8_t rxBufferSize)
{
    if (rxBuffer != NULL && rxBufferSize > 0)
    {
        pThis->_rxBuffer = rxBuffer;
        pThis->_rxBufferSize = rxBufferSize;
        pThis->_rxDataCount = 0;
    }
}

PUBLIC void setEthernetTxBuffer(struct Ethernet * pThis, 
    uint8_t * txBuffer, uint8_t txBufferSize)
{
    if (txBuffer != NULL && txBufferSize > 0)
    {
        pThis->_txBuffer = txBuffer;
        pThis->_txBufferSize = txBufferSize;
        pThis->_txDataCount = 0;
    }
}

PUBLIC VIRTUAL void defaultOnEthernetReceive(struct Ethernet * pThis)
{
    pThis->send(pThis, pThis->_rxBuffer, pThis->_rxDataCount);
    pThis->_rxDataCount = 0; // reset rx data counter
}

PUBLIC VIRTUAL uint8_t defaultEthernetSend(struct Ethernet * pThis, 
    uint8_t * buffer, uint8_t length)
{
    uint8_t txDataCount = length;

    if (!IS_ETHERNET_TX_BUFFER_EMPTY(pThis) ||
        !IS_ETHERNET_BUFFER_VALID(buffer, length))
    {
        return 0;
    }

    // get max data length to send
    if (txDataCount > pThis->_txBufferSize)
    {
        txDataCount = pThis->_txBufferSize;
    }

    // copy data to tx buffer
    memcpy(pThis->_txBuffer, buffer, txDataCount);
    pThis->_txDataCount = txDataCount;

    return txDataCount;
}

PUBLIC STATIC uint8_t * getEthernetLocalMAC(void)
{
    static uint8_t mac[ETHERNET_MAC_LENGTH] = {0};

    if (localMAC == NULL)
    {
        uint32_t serial[2] = {
            [0] = *(uint32_t *)0x1FFFF7E8,
            [1] = *(uint32_t *)0x1FFFF7EC
        };

        *((uint32_t *)&mac) = serial[0] << 2;
        *(((uint32_t *)&mac) + 1) = ((serial[1] >> 30) & 0x03) 
                + ((serial[1] << 2) & 0xFFFFFFFC);

        localMAC = mac;
    }

    return localMAC;
}
