#ifndef __LED_H__
#define __LED_H__

#include "gpiopin.h"
#include "keywords.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct LED {
    GPIOPin _pin;
    GPIOPinState _statLight;
} LED;

// (de)constructor(s)
PUBLIC LED newLED(GPIOPin pin, GPIOPinState lightState);

// public method(s)
PUBLIC void lightUpLED(LED * this);
PUBLIC void blackOutLED(LED * this);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LED_H__
