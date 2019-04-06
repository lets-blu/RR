#include "ethernet.h"

PUBLIC struct Ethernet newEthernet(void)
{
    struct Ethernet ethernet = {
        ._serverPort = 0,
        ._serverIP = {0, 0, 0, 0},

        ._clientPort = 0,
        ._clientIP = {0, 0, 0, 0},

        ._subnetMask = {0, 0, 0, 0},
        ._gatewayIP = {0, 0, 0, 0},

        ._rxBuffer = NULL,
        ._rxBufferSize = 0,
        ._numOfRxData = 0,

        ._txBuffer = NULL,
        ._txBufferSize = 0,
        ._numOfTxData = 0,

        .init = defaultEthernetInit,
        .send = defaultEthernetSend,
        .onReceive = defaultOnEthernetReceive};

    return ethernet;
}

PUBLIC void setEthernetServerParams(struct Ethernet *pThis,
                                    uint8_t serverIP[ETHERNET_IP_LEN], uint16_t serverPort)
{
    memcpy(pThis->_serverIP, serverIP, ETHERNET_IP_LEN * sizeof(uint8_t));
    pThis->_serverPort = serverPort;
}

PUBLIC void setEthernetClientParams(struct Ethernet *pThis,
                                    uint8_t clientIP[], uint16_t clientPort)
{
    memcpy(pThis->_clientIP, clientIP, ETHERNET_IP_LEN * sizeof(uint8_t));
    pThis->_clientPort = clientPort;
}

PUBLIC void setEthernetNetworkParams(struct Ethernet *pThis,
                                     uint8_t subnetMask[], uint8_t gatewayIP[])
{
    memcpy(pThis->_subnetMask, subnetMask, ETHERNET_IP_LEN * sizeof(uint8_t));
    memcpy(pThis->_gatewayIP, gatewayIP, ETHERNET_IP_LEN * sizeof(uint8_t));
}

PUBLIC void setEthernetRxBuffer(struct Ethernet *pThis,
                                uint8_t *rxBuffer, uint8_t rxBufferSize)
{
    pThis->_rxBuffer = rxBuffer;
    pThis->_rxBufferSize = rxBufferSize;
    pThis->_numOfRxData = 0;
}

PUBLIC void setEthernetTxBuffer(struct Ethernet *pThis,
                                uint8_t *txBuffer, uint8_t txBufferSize)
{
    pThis->_txBuffer = txBuffer;
    pThis->_txBufferSize = txBufferSize;
    pThis->_numOfTxData = 0;
}

PUBLIC VIRTUAL bool defaultEthernetInit(struct Ethernet *pThis)
{
    return false;
}

PUBLIC VIRTUAL uint8_t defaultEthernetSend(struct Ethernet *pThis,
                                           uint8_t message[], uint8_t length)
{
    if (pThis->_txBuffer == NULL || pThis->_numOfTxData > 0)
    {
        return 0;
    }

    memcpy(pThis->_txBuffer, message, length * sizeof(uint8_t));
    pThis->_numOfTxData = length;

    return length;
}

PUBLIC VIRTUAL void defaultOnEthernetReceive(struct Ethernet *pThis)
{
    pThis->send(pThis, pThis->_rxBuffer, pThis->_numOfRxData);
    pThis->_numOfRxData = 0;
}
