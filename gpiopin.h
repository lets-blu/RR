#ifndef __GPIOPIN_H__
#define __GPIOPIN_H__

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "keywords.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef enum {
    LOW     = 0, 
    HIGH    = 1
} GPIOPinState;

typedef struct GPIOPin {
    GPIO_TypeDef * _port;
    uint16_t _pin;
} GPIOPin;

PUBLIC GPIOPin newGPIOPin(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin_x);

PUBLIC void configGPIOPin(GPIOPin * this, 
        GPIOSpeed_TypeDef GPIO_Speed, GPIOMode_TypeDef GPIO_Mode);
PUBLIC GPIOPinState readGPIOPin(GPIOPin * this);
PUBLIC void writeGPIOPin(GPIOPin * this, GPIOPinState state);
PUBLIC void getGPIOPinPortSource(GPIOPin * this, uint8_t * portSource);
PUBLIC void getGPIOPinPinSource(GPIOPin * this, uint8_t * pinSource);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __GPIOPIN_H__
