#ifndef __W5500CLASS_H__
#define __W5500CLASS_H__

#include "stdbool.h"
#include "stdlib.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "string.h"
#include "wizchip_conf.h"

#include "ethernet.h"
#include "gpiopin.h"
#include "keywords.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    typedef struct W5500 {
        struct Ethernet base; // super class

        SPI_TypeDef * _spi;

        // these function pointers are used to initialize w5500 chip
        void(*cris_en)(void);
        void(*cris_ex)(void);

        void(*cs_sel)(void);
        void(*cs_desel)(void);

        uint8_t(*spi_rb)(void);
        void(*spi_wb)(uint8_t wb);
    } W5500;


    // (de)constructor(s)
    PUBLIC W5500 newW5500(SPI_TypeDef * SPIx);

    // public method(s)
    PUBLIC void syncW5500(W5500 * pThis);
    PUBLIC VIRTUAL bool initW5500(W5500 * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __W5500CLASS_H__
