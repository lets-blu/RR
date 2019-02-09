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
        LOW = 0, HIGH
    } GPIOPinState;

    typedef struct GPIOPin {
        GPIO_TypeDef * _port;
        uint16_t _pin;
    } GPIOPin;

    // (de)constructor(s)
    PUBLIC GPIOPin newGPIOPin(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin_x);

    // public method(s)
    PUBLIC void configGPIOPin(GPIOPin * pThis,
        GPIOSpeed_TypeDef GPIO_Speed, GPIOMode_TypeDef GPIO_Mode);
    PUBLIC GPIOPinState readGPIOPin(GPIOPin * pThis);
    PUBLIC void writeGPIOPin(GPIOPin * pThis, GPIOPinState state);
    PUBLIC void getGPIOPinPortSource(GPIOPin * pThis, uint8_t * portSource);
    PUBLIC void getGPIOPinPinSource(GPIOPin * pThis, uint8_t * pinSource);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __GPIOPIN_H__
