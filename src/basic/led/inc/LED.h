#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "core/device/inc/DeviceManager.h"
#include "core/log/inc/LogFilter.h"

#include "port/common/inc/BasePin.h"

typedef struct {
    BasePin *_basePin;
    PinState _lightState;
} LED;

// (De)constructors
PUBLIC void constructLED(
    LED *instance, void *port, unsigned int pin, PinState lightState);

PUBLIC void deconstructLED(LED *instance);

// Public method(s)
PUBLIC void turnOnLED(LED *pThis);
PUBLIC void turnOffLED(LED *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LED_H__

