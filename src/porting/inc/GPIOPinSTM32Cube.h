#ifndef __GPIO_PIN_STM32CUBE__
#define __GPIO_PIN_STM32CUBE__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef STM32F411xE
#include "stm32f4xx_hal.h"
#endif

#define INPUT           GPIO_MODE_INPUT
#define OUTPUT          GPIO_MODE_OUTPUT_PP
#define INPUT_PULLUP    GPIO_MODE_OUTPUT_PP
typedef uint32_t GPIOPinMode;

typedef GPIO_TypeDef GPIOPinPort;
typedef uint16_t GPIOPinPin;
typedef GPIO_PinState GPIOPinState;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __GPIO_PIN_ST32CUBE__
