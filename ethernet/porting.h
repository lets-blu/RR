#ifndef __PORTING_H__
#define __PORTING_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"

void SPI2_Cris_Enter(void);
void SPI2_Cris_Exit(void);

void SPI2_CS_Select(void);
void SPI2_CS_Deselect(void);

uint8_t SPI2_ReadByte(void);
void SPI2_WriteByte(uint8_t wb);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PORTING_H__
