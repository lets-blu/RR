#include "w5500.h"

#define __SOCK_TCP      ((uint8_t)0)

// private method
PRIVATE void w5500Send(W5500 * pThis);
PRIVATE void w5500Receive(W5500 * pThis);

PRIVATE STATIC void initW5500SPI(SPI_TypeDef * SPIx);
PRIVATE STATIC void initW5500SPIPins(SPI_TypeDef * SPIx);

PUBLIC W5500 newW5500(SPI_TypeDef * SPIx)
{
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    W5500 w5500 = {
        .base       = newEthernet(),

        ._spi       = SPIx,

        .cris_en    = NULL,
        .cris_ex    = NULL,

        .cs_sel     = NULL,
        .cs_desel   = NULL,

        .spi_rb     = NULL,
        .spi_wb     = NULL
    };

    w5500.base.init = (bool(*)(struct Ethernet *))initW5500;
    
    return w5500;
}

PUBLIC void syncW5500(W5500 * pThis)
{
    switch (getSn_SR(__SOCK_TCP)) {
    case SOCK_ESTABLISHED:
        if (getSn_IR(__SOCK_TCP) & Sn_IR_CON) {
            setSn_IR(__SOCK_TCP, Sn_IR_CON);
        }

        w5500Send(pThis);
        w5500Receive(pThis);
        break;

    case SOCK_CLOSE_WAIT:
        disconnect(__SOCK_TCP);
        break;

    case SOCK_INIT:
        connect(__SOCK_TCP,
            pThis->base._serverIP, pThis->base._serverPort);
        break;

    case SOCK_CLOSED:
        close(__SOCK_TCP);
        socket(__SOCK_TCP, Sn_MR_TCP, pThis->base._clientPort, 0x00);
        break;

    default:
        break;
    }
}

PUBLIC VIRTUAL bool initW5500(W5500 * pThis)
{
    initW5500SPI(pThis->_spi);

    reg_wizchip_cris_cbfunc(pThis->cris_en, pThis->cris_ex);
    reg_wizchip_cs_cbfunc(pThis->cs_sel, pThis->cs_desel);
    reg_wizchip_spi_cbfunc(pThis->spi_rb, pThis->spi_wb);

    // WIZCHIP SOCKET Buffer initialize
    uint8_t memsize[2][8] = { {2,2,2,2,2,2,2,2}, {2,2,2,2,2,2,2,2} };
    if (ctlwizchip(CW_INIT_WIZCHIP, (void *)memsize) == -1) {
        return  false;
    }

    wiz_NetInfo netInfo = { .dhcp = NETINFO_STATIC };
    memcpy(netInfo.mac, ETHERNET_CLIENT_MAC, ETHERNET_MAC_LEN * sizeof(uint8_t));
    memcpy(netInfo.ip, pThis->base._clientIP, ETHERNET_IP_LEN * sizeof(uint8_t));
    memcpy(netInfo.sn, pThis->base._subnetMask, ETHERNET_IP_LEN * sizeof(uint8_t));
    memcpy(netInfo.gw, pThis->base._gatewayIP, ETHERNET_IP_LEN * sizeof(uint8_t));
    ctlnetwork(CN_SET_NETINFO, (void*)&netInfo);

#ifndef DEBUG
    wiz_NetTimeout netTimeOut = { .retry_cnt = 3,.time_100us = 1000 };
    wizchip_settimeout(&netTimeOut);
#endif

    return true;
}

PRIVATE void w5500Send(W5500 * pThis)
{
    if (pThis->base._txBuffer == NULL || pThis->base._numOfTxData == 0) {
        return;
    }

    if (send(__SOCK_TCP, pThis->base._txBuffer, pThis->base._numOfTxData) < 0) {
        close(__SOCK_TCP);
        return;
    }

    pThis->base._numOfTxData = 0;
}

PRIVATE void w5500Receive(W5500 * pThis)
{
    uint16_t length = getSn_RX_RSR(__SOCK_TCP);
    if (length == 0 
        || pThis->base._rxBuffer == NULL || pThis->base._numOfRxData != 0) {
        return;
    }

    if (length > pThis->base._rxBufferSize) {
        length = pThis->base._rxBufferSize;
    }

    if (recv(__SOCK_TCP, pThis->base._rxBuffer, length) > 0) {
        pThis->base._numOfRxData = length;
        pThis->base.onReceive((struct Ethernet *)pThis);
    }
}

PRIVATE STATIC void initW5500SPI(SPI_TypeDef * SPIx)
{
    initW5500SPIPins(SPIx);

    SPI_InitTypeDef SPI_InitStructure = {
        .SPI_BaudRatePrescaler  = SPI_BaudRatePrescaler_2,
        .SPI_Direction          = SPI_Direction_2Lines_FullDuplex,
        .SPI_Mode               = SPI_Mode_Master,
        .SPI_DataSize           = SPI_DataSize_8b,
        .SPI_CPOL               = SPI_CPOL_Low,
        .SPI_CPHA               = SPI_CPHA_1Edge,
        .SPI_NSS                = SPI_NSS_Soft,
        .SPI_FirstBit           = SPI_FirstBit_MSB,
        .SPI_CRCPolynomial      = 7
    };
    
    if (SPIx == SPI1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    }
    else {
        if (SPIx == SPI2) {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
        }
    }

    SPI_Init(SPIx, &SPI_InitStructure);
    SPI_SSOutputCmd(SPIx, ENABLE);
    SPI_Cmd(SPIx, ENABLE);
}

PRIVATE STATIC void initW5500SPIPins(SPI_TypeDef * SPIx)
{
    GPIOPin nssPin, sckPin, misoPin, mosiPin;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    if (SPIx == SPI1) {
        nssPin = newGPIOPin(GPIOA, GPIO_Pin_4);
        sckPin = newGPIOPin(GPIOA, GPIO_Pin_5);
        misoPin = newGPIOPin(GPIOA, GPIO_Pin_6);
        mosiPin = newGPIOPin(GPIOA, GPIO_Pin_7);
    }
    else {
        if (SPIx == SPI2) {
            nssPin = newGPIOPin(GPIOB, GPIO_Pin_12);
            sckPin = newGPIOPin(GPIOB, GPIO_Pin_13);
            misoPin = newGPIOPin(GPIOB, GPIO_Pin_14);
            mosiPin = newGPIOPin(GPIOB, GPIO_Pin_15);
        }
    }

    setupGPIOPin(&nssPin, OUTPUT);
    setupGPIOPin(&sckPin, OUTPUT_ALTERNATE);
    setupGPIOPin(&misoPin, OUTPUT_ALTERNATE);
    setupGPIOPin(&mosiPin, OUTPUT_ALTERNATE);
}

PUBLIC STATIC void vSyncW5500Task(void * pW5500)
{
    W5500 * w5500 = (W5500 *)pW5500;

    for (;;) {
        syncW5500(w5500);
    }
}
