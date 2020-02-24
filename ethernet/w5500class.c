#include "w5500class.h"

#define W5500_DHCP_SOCKET   0
#define W5500_TCPC_SOCKET   1

#define IS_W5500_RX_BUFFER_EMPTY(base) \
    ((base)->_rxBuffer != NULL && (base)->_rxDataCount == 0)

#define IS_W5500_TX_BUFFER_EMPTY(base) \
    ((base)->_txBuffer != NULL && (base)->_txDataCount == 0)

// Static member(s)
PRIVATE STATIC GPIOPin w5500NSSPin;
PRIVATE STATIC SPI_HandleTypeDef w5500SPIHandle;
PRIVATE STATIC osThreadId w5500TimerThread;

// Private method(s)
PRIVATE bool initW5500(W5500Class * pThis);
PRIVATE void syncW5500(W5500Class * pThis);
PRIVATE void recvW5500(W5500Class * pThis);
PRIVATE void sendW5500(W5500Class * pThis);
PRIVATE void enableW5500Sync(W5500Class * pThis);
PRIVATE void disableW5500Sync(W5500Class * pThis);

PRIVATE STATIC void initW5500SPI(SPI_TypeDef * SPIx);
PRIVATE STATIC void registerW5500Callbacks(SPI_TypeDef * SPIx);

PRIVATE STATIC void enterW5500Critical(void);
PRIVATE STATIC void exitW5500Critical(void);
PRIVATE STATIC void selectW5500Chip(void);
PRIVATE STATIC void deselectW5500Chip(void);
PRIVATE STATIC uint8_t readW5500SPIByte(void);
PRIVATE STATIC void writeW5500SPIByte(uint8_t wb);
PRIVATE STATIC void onW5500IPAssign(void);
PRIVATE STATIC void onW5500IPConflict(void);

PUBLIC W5500Class newW5500(SPI_TypeDef * SPIx)
{
    assert(IS_SPI_ALL_INSTANCE(SPIx));

    W5500Class w5500 = {
        .base           = newEthernet(),
        ._syncThread    = NULL
    };

    initW5500SPI(SPIx);
    registerW5500Callbacks(SPIx);
    
    return w5500;
}

PUBLIC bool isW5500SyncEnabled(W5500Class * pThis)
{
    return ((pThis->_syncThread != NULL)
        && (osThreadGetState(pThis->_syncThread) != osThreadDeleted));
}

PUBLIC void setW5500SyncEnabled(W5500Class * pThis, bool enabled)
{
    if (enabled)
    {
        enableW5500Sync(pThis);
    }
    else
    {
        disableW5500Sync(pThis);
    }
}

PRIVATE bool initW5500(W5500Class * pThis)
{
    uint8_t linkStatus = PHY_LINK_OFF;
    uint8_t memsize[2][8] = {{2, 2, 2, 2, 2, 2, 2, 2}, {2, 2, 2, 2, 2, 2, 2, 2}};

    if (ctlwizchip(CW_INIT_WIZCHIP, memsize) == -1)
    {
        return false;
    }
    
    while (linkStatus == PHY_LINK_OFF)
    {
        if (ctlwizchip(CW_GET_PHYLINK, &linkStatus) == -1)
        {
            return false;
        }
    }

    if (pThis->base._dhcpEnabled)
    {
        setSHAR(getEthernetLocalMAC());
        reg_dhcp_cbfunc(onW5500IPAssign, onW5500IPAssign, onW5500IPConflict);
        
        DHCP_init(W5500_DHCP_SOCKET, pThis->base._dhcpBuffer);
    }
    else
    {
        wiz_NetInfo net_info = {
            .dhcp = NETINFO_STATIC
        };

        memcpy(net_info.ip, pThis->base._localIP, ETHERNET_IP_LENGTH);
        memcpy(net_info.sn, pThis->base._subnetMask, ETHERNET_IP_LENGTH);
        memcpy(net_info.gw, pThis->base._defaultGateway, ETHERNET_IP_LENGTH);
        memcpy(net_info.mac, getEthernetLocalMAC(), ETHERNET_MAC_LENGTH);

        ctlnetwork(CN_SET_NETINFO, &net_info);
    }

    return true;
}

PRIVATE void syncW5500(W5500Class * pThis)
{
    struct Ethernet * base = &pThis->base;
    
    switch (getSn_SR(W5500_TCPC_SOCKET))
    {
        case SOCK_CLOSED:
        {
            socket(W5500_TCPC_SOCKET, Sn_MR_TCP, base->_localPort, 0);
            break;
        }

        case SOCK_INIT:
        {
            connect(W5500_TCPC_SOCKET, base->_serverIP, base->_serverPort);
            break;
        }

        case SOCK_ESTABLISHED:
        {
            if (getSn_IR(W5500_TCPC_SOCKET) & Sn_IR_CON)
            {
                setSn_IR(W5500_TCPC_SOCKET, Sn_IR_CON);
            }

            recvW5500(pThis);
            sendW5500(pThis);

            break;
        }

        case SOCK_CLOSE_WAIT:
        {
            disconnect(W5500_TCPC_SOCKET);
            break;
        }
    }
}

PRIVATE void recvW5500(W5500Class * pThis)
{
    struct Ethernet * base = &pThis->base;
    uint16_t rxDataCount = getSn_RX_RSR(W5500_TCPC_SOCKET);
    
    if (rxDataCount == 0 || !IS_W5500_RX_BUFFER_EMPTY(base))
    {
        return;
    }

    // get max length to receive
    if (rxDataCount > base->_rxBufferSize)
    {
        rxDataCount = base->_rxBufferSize;
    }

    // receive data from W5500
    if (recv(W5500_TCPC_SOCKET, base->_rxBuffer, rxDataCount) > 0)
    {
        pThis->base._rxDataCount = rxDataCount;
        pThis->base.onReceive(base);
    }
}

PRIVATE void sendW5500(W5500Class * pThis)
{
    struct Ethernet * base = &pThis->base;
    
    if (IS_W5500_TX_BUFFER_EMPTY(base))
    {
        return;
    }

    if (send(W5500_TCPC_SOCKET, base->_txBuffer, base->_txDataCount) < 0)
    {
        close(W5500_TCPC_SOCKET);
        return;
    }

    base->_txDataCount = 0; // reset tx buffer counter
}

PRIVATE void enableW5500Sync(W5500Class * pThis)
{
    if (isW5500SyncEnabled(pThis))
    {
        return;
    }
    
    if (pThis->base._dhcpEnabled && w5500TimerThread == NULL)
    {
        osThreadDef(w5500TimerThread, vW5500TimerThread, osPriorityNormal, 0, 128);
        w5500TimerThread = osThreadCreate(osThread(w5500TimerThread), NULL);
    }
    
    osThreadDef(syncThread, vSyncW5500Thread, osPriorityNormal, 0, 128);
    pThis->_syncThread = osThreadCreate(osThread(syncThread), pThis);
}

PRIVATE void disableW5500Sync(W5500Class * pThis)
{
    if (isW5500SyncEnabled(pThis))
    {
        osThreadTerminate(pThis->_syncThread);
    }
}

PUBLIC STATIC void vSyncW5500Thread(const void * parameter)
{
    W5500Class * w5500 = (W5500Class *)parameter;
    bool enableDHCP = w5500->base._dhcpEnabled;

    if (!initW5500(w5500))
    {
        osThreadTerminate(NULL);
    }
    
    LOOP
    {
        if ((!enableDHCP) || (enableDHCP && DHCP_run() == DHCP_IP_LEASED))
        {
            syncW5500(w5500);
        }
    }
}

PUBLIC STATIC void vW5500TimerThread(const void * parameter)
{
    LOOP
    {
        DHCP_time_handler();
        osDelay(1000);
    }
}

PRIVATE STATIC void initW5500SPI(SPI_TypeDef * SPIx)
{
    w5500SPIHandle.Instance = SPIx;
    w5500SPIHandle.Init.Mode = SPI_MODE_MASTER;
    w5500SPIHandle.Init.Direction = SPI_DIRECTION_2LINES;
    w5500SPIHandle.Init.DataSize = SPI_DATASIZE_8BIT;
    w5500SPIHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
    w5500SPIHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
    w5500SPIHandle.Init.NSS = SPI_NSS_SOFT;
    w5500SPIHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    w5500SPIHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    w5500SPIHandle.Init.TIMode = SPI_TIMODE_DISABLE;
    w5500SPIHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    w5500SPIHandle.Init.CRCPolynomial = 10;

    if (SPIx == SPI1)
    {
        w5500NSSPin = newGPIOPin(GPIOA, GPIO_PIN_4);
    }
    else
    {
        w5500NSSPin = newGPIOPin(GPIOB, GPIO_PIN_12);
    }

    HAL_SPI_Init(&w5500SPIHandle);
    setupGPIOPin(&w5500NSSPin, OUTPUT);
}

PRIVATE STATIC void registerW5500Callbacks(SPI_TypeDef * SPIx)
{
    reg_wizchip_cris_cbfunc(enterW5500Critical, exitW5500Critical);
    reg_wizchip_cs_cbfunc(selectW5500Chip, deselectW5500Chip);
    reg_wizchip_spi_cbfunc(readW5500SPIByte, writeW5500SPIByte);
}

PRIVATE STATIC void enterW5500Critical(void)
{
    __set_PRIMASK(1);
}

PRIVATE STATIC void exitW5500Critical(void)
{
    __set_PRIMASK(0);
}

PRIVATE STATIC void selectW5500Chip(void)
{
    writeGPIOPin(&w5500NSSPin, LOW);
}

PRIVATE STATIC void deselectW5500Chip(void)
{
    writeGPIOPin(&w5500NSSPin, HIGH);
}

PRIVATE STATIC uint8_t readW5500SPIByte(void)
{
    uint8_t rb = 0;
    HAL_SPI_Receive(&w5500SPIHandle, &rb, 1, 0xFFFF);

    return rb;
}

PRIVATE STATIC void writeW5500SPIByte(uint8_t wb)
{
    HAL_SPI_Transmit(&w5500SPIHandle, &wb, 1, 0xFFFF);
}

PRIVATE STATIC void onW5500IPAssign(void)
{
    wiz_NetInfo net_info = {
        .dhcp = NETINFO_DHCP
    };

    getIPfromDHCP(net_info.ip);
    getGWfromDHCP(net_info.gw);
    getSNfromDHCP(net_info.sn);
    getDNSfromDHCP(net_info.dns);
    memcpy(net_info.mac, getEthernetLocalMAC(), ETHERNET_MAC_LENGTH);

    ctlnetwork(CN_SET_NETINFO, &net_info);
}

PRIVATE STATIC void onW5500IPConflict(void)
{
    // do nothing
}
