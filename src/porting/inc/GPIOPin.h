#ifndef __GPIO_PIN_H__
#define __GPIO_PIN_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "utility/inc/Keywords.h"

#if defined(ARDUINO)
#include "porting/inc/GPIOPinArduino.h"
#elif defined(USE_HAL_DRIVER)
#include "porting/inc/GPIOPinSTM32Cube.h"
#endif

typedef struct {
    GPIOPinPin _pin;
    GPIOPinPort _port;
} GPIOPin;

// (De)constructors
PUBLIC void constructGPIOPin(GPIOPin *instance, GPIOPinPort port, GPIOPinPin pin);
PUBLIC void deconstructGPIOPin(GPIOPin *instance);

// Public method(s)
PUBLIC void setupGPIOPin(GPIOPin *pThis, GPIOPinMode mode);
PUBLIC GPIOPinState readFromGPIOPin(GPIOPin *pThis);
PUBLIC void writeToGPIOPin(GPIOPin *pThis, GPIOPinState state);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __GPIO_PIN_H__
