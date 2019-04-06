#ifndef __GPIOPIN_H__
#define __GPIOPIN_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "assert.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "keywords.h"

#define INPUT GPIO_Mode_IN_FLOATING
#define OUTPUT GPIO_Mode_Out_PP
#define INPUT_PULLUP GPIO_Mode_IPU
#define OUTPUT_ALTERNATE GPIO_Mode_AF_PP

#define LOW Bit_RESET
#define HIGH Bit_SET

    typedef struct GPIOPin
    {
        GPIO_TypeDef *_port;
        uint16_t _pin;
    } GPIOPin;

    typedef GPIOMode_TypeDef GPIOPinMode;
    typedef BitAction GPIOPinState;

    // (de)constructor(s)
    PUBLIC GPIOPin newGPIOPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin_x);

    // public method(s)
    PUBLIC void setupGPIOPin(GPIOPin *pThis, GPIOPinMode mode);
    PUBLIC GPIOPinState readGPIOPin(GPIOPin *pThis);
    PUBLIC void writeGPIOPin(GPIOPin *pThis, GPIOPinState state);

    PUBLIC uint8_t getGPIOPinPortSource(GPIOPin *pThis);
    PUBLIC uint8_t getGPIOPinPinSource(GPIOPin *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __GPIOPIN_H__
