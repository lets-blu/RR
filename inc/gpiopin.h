#ifndef __GPIOPIN_H__
#define __GPIOPIN_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdint.h"

#include "stm32f1xx.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"

#include "keywords.h"

#define PULLUP              0x00001000U

#define INPUT               GPIO_MODE_INPUT
#define OUTPUT              GPIO_MODE_OUTPUT_PP
#define INPUT_PULLUP        (GPIO_MODE_INPUT | PULLUP)

#define INTERRUPT_CHANGE    GPIO_MODE_EVT_RISING_FALLING
#define INTERRUPT_RISING    GPIO_MODE_IT_RISING
#define INTERRUPT_FALLING   GPIO_MODE_IT_FALLING

#define LOW                 GPIO_PIN_RESET
#define HIGH                GPIO_PIN_SET

typedef struct 
{
    GPIO_TypeDef * _port;
    uint16_t _pin;
} GPIOPin;

typedef uint32_t GPIOPinMode;
typedef GPIO_PinState GPIOPinState;

// (de)constructor(s)
PUBLIC GPIOPin newGPIOPin(GPIO_TypeDef * GPIOx, uint16_t GPIO_PIN_x);

// public method(s)
PUBLIC void setupGPIOPin(GPIOPin * pThis, GPIOPinMode mode);

PUBLIC GPIOPinState readGPIOPin(GPIOPin * pThis);
PUBLIC void writeGPIOPin(GPIOPin * pThis, GPIOPinState state);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __GPIOPIN_H__
