#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "gpiopin.h"
#include "keywords.h"

typedef struct {
    GPIOPin _pin;
    GPIOPinState _lightState;
} LED;

// Constructor
PUBLIC LED newLED(GPIOPin pin, GPIOPinState lightState);

// Public method(s)
PUBLIC void turnOnLED(LED * pThis);
PUBLIC void turnOffLED(LED * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LED_H__
