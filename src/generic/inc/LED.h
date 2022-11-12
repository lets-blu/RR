#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "porting/inc/GPIOPin.h"
#include "syslog/inc/LogFilter.h"
#include "utility/inc/Keywords.h"

typedef struct {
    GPIOPin _pin;
    GPIOPinState _lightState;
} LED;

// (De)constructors
PUBLIC void constructLED(LED *instance, GPIOPin *pin, GPIOPinState lightState);
PUBLIC void deconstructLED(LED *instance);

// Public methods(s)
PUBLIC void turnOnLED(LED *pThis);
PUBLIC void turnOffLED(LED *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LED_H__
