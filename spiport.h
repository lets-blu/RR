#ifndef __SPIPORT_H__
#define __SPIPORT_H__

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"

#include "gpiopin.h"
#include "keywords.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef SPI_TypeDef * SPIPort;

// (de)constructor(s)
PUBLIC SPIPort newSPIPort(SPI_TypeDef * spi);

// public method(s)
PUBLIC void initSPIPort(SPIPort * this);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SPIPORT_H__
