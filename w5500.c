#include "w5500.h"
#include "socket.h"

#define __SOCK_TCP      ((uint8_t)0)

#define __NUM_OF_PINS   ((uint8_t)4)
#define __NSS_PIN       ((uint8_t)0)
#define __SCK_PIN       ((uint8_t)1)
#define __MISO_PIN      ((uint8_t)2)
#define __MOSI_PIN      ((uint8_t)3)

// private method
PRIVATE void w5500Send(W5500 * pThis);
PRIVATE void w5500Receive(W5500 * pThis);
PRIVATE STATIC void initW5500SPI(SPI_TypeDef * SPIx);
PRIVATE STATIC void initW5500SPIPins(SPI_TypeDef * SPIx);
PRIVATE STATIC void setW5500SPIClock(SPI_TypeDef * SPIx, FunctionalState newState);

PUBLIC W5500 newW5500(SPI_TypeDef * SPIx) {
    W5500 w5500 = {
        .base = newEthernet(),
        ._spi = SPIx,
        .cris_en = NULL,
        .cris_ex = NULL,
        .cs_sel = NULL,
        .cs_desel = NULL,
        .spi_rb = NULL,
        .spi_wb = NULL
    };

    w5500.base.init = (bool(*)(struct Ethernet *))initW5500;
    initW5500SPI(SPIx);

    return w5500;
}

PUBLIC void syncW5500(W5500 * pThis) {
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

PUBLIC VIRTUAL bool initW5500(W5500 * pThis) {
    reg_wizchip_cris_cbfunc(pThis->cris_en, pThis->cris_ex);
    reg_wizchip_cs_cbfunc(pThis->cs_sel, pThis->cs_desel);
    reg_wizchip_spi_cbfunc(pThis->spi_rb, pThis->spi_wb);

    // WIZCHIP SOCKET Buffer initialize
    uint8_t memsize[2][8] = { {2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2} };
    if (ctlwizchip(CW_INIT_WIZCHIP, (void*)memsize) == -1) {
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

PRIVATE void w5500Send(W5500 * pThis) {
    if (!(pThis->base._txBuffer) || !(pThis->base._numOfTxData)) {
        return;
    }

    if (send(__SOCK_TCP, pThis->base._txBuffer, pThis->base._numOfTxData) < 0) {
        close(__SOCK_TCP);
        return;
    }

    pThis->base._numOfTxData = 0;
}

PRIVATE void w5500Receive(W5500 * pThis) {
    uint16_t length = getSn_RX_RSR(__SOCK_TCP);
    if (!length || !(pThis->base._rxBuffer) || pThis->base._numOfRxData) {
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

PRIVATE STATIC void initW5500SPI(SPI_TypeDef * SPIx) {
    initW5500SPIPins(SPIx);
    setW5500SPIClock(SPIx, ENABLE);

    SPI_InitTypeDef SPI_InitStructure = {
        .SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2,
        .SPI_Direction = SPI_Direction_2Lines_FullDuplex,
        .SPI_Mode = SPI_Mode_Master,
        .SPI_DataSize = SPI_DataSize_8b,
        .SPI_CPOL = SPI_CPOL_Low,
        .SPI_CPHA = SPI_CPHA_1Edge,
        .SPI_NSS = SPI_NSS_Soft,
        .SPI_FirstBit = SPI_FirstBit_MSB,
        .SPI_CRCPolynomial = 7
    };

    SPI_Init(SPIx, &SPI_InitStructure);
    SPI_SSOutputCmd(SPIx, ENABLE);
    SPI_Cmd(SPIx, ENABLE);
}

PRIVATE STATIC void initW5500SPIPins(SPI_TypeDef * SPIx) {
    GPIOPin pins[__NUM_OF_PINS];

    if (SPIx == SPI1) {
        pins[__NSS_PIN] = newGPIOPin(GPIOA, GPIO_Pin_4);
        pins[__SCK_PIN] = newGPIOPin(GPIOA, GPIO_Pin_5);
        pins[__MISO_PIN] = newGPIOPin(GPIOA, GPIO_Pin_6);
        pins[__MOSI_PIN] = newGPIOPin(GPIOA, GPIO_Pin_7);
    }
    else {
        if (SPIx == SPI2) {
            pins[__NSS_PIN] = newGPIOPin(GPIOB, GPIO_Pin_12);
            pins[__SCK_PIN] = newGPIOPin(GPIOB, GPIO_Pin_13);
            pins[__MISO_PIN] = newGPIOPin(GPIOB, GPIO_Pin_14);
            pins[__MOSI_PIN] = newGPIOPin(GPIOB, GPIO_Pin_15);
        }
    }

    for (uint8_t i = 0; i < __NUM_OF_PINS; i++) {
        switch (i) {
        case __NSS_PIN:
            configGPIOPin(pins + i, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
            writeGPIOPin(pins + i, HIGH);
            break;

        case __SCK_PIN:
        case __MISO_PIN:
        case __MOSI_PIN:
            configGPIOPin(pins + i, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);
            break;
        }
    }
}

PRIVATE STATIC void setW5500SPIClock(SPI_TypeDef * SPIx, FunctionalState newState) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    if (SPIx == SPI1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, newState);
    }
    else {
        if (SPIx == SPI2) {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, newState);
        }
    }
}
