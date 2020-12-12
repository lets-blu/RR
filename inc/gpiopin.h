#ifndef __GPIOPIN_H__
#define __GPIOPIN_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "assert.h"
#include "stdint.h"

#include "keywords.h"
#include "testing.h"

#ifdef UNIT_TEST
#include "hal.h"
#include "hal_gpio.h"
#include "hal_rcc.h"
#else
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#endif // UNIT_TEST

#define PULLUP              0x00001000U

#define INPUT               GPIO_MODE_INPUT
#define OUTPUT              GPIO_MODE_OUTPUT_PP
#define INPUT_PULLUP        (GPIO_MODE_INPUT | PULLUP)

#define INTERRUPT_CHANGE    GPIO_MODE_IT_RISING_FALLING
#define INTERRUPT_RISING    GPIO_MODE_IT_RISING
#define INTERRUPT_FALLING   GPIO_MODE_IT_FALLING

#define LOW                 GPIO_PIN_RESET
#define HIGH                GPIO_PIN_SET

typedef struct {
    uint16_t _pin;
    GPIO_TypeDef * _port;
} GPIOPin;

typedef uint32_t GPIOPinMode;
typedef GPIO_PinState GPIOPinState;

// Constructor
PUBLIC GPIOPin newGPIOPin(GPIO_TypeDef * GPIOx, uint16_t GPIO_PIN_x);

// Public method(s)
PUBLIC void setupGPIOPin(GPIOPin * pThis, GPIOPinMode mode);

PUBLIC GPIOPinState readGPIOPin(GPIOPin * pThis);
PUBLIC void writeGPIOPin(GPIOPin * pThis, GPIOPinState state);
PUBLIC void writeGPIOPinValue(GPIOPin * pThis, uint16_t value);

PUBLIC GPIO_TypeDef * getGPIOPinPort(GPIOPin * pThis);
PUBLIC uint16_t getGPIOPinPin(GPIOPin * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __GPIOPIN_H__
