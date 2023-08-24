#ifndef __BASE_PIN_H__
#define __BASE_PIN_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "core/device/inc/BaseDevice.h"

#define BASE_PIN_VALUE_LOW  0
#define BASE_PIN_VALUE_HIGH 1

#define BaseDevice2BasePin(instance) \
    BASE_TO_SUB_CAST(instance, BasePin, base)

struct BasePinVtbl;

typedef struct {
    BaseDevice base;

    void *_port;
    unsigned int _pin;
    const struct BasePinVtbl *vtbl;
} BasePin;

typedef struct {
    void *port;
    unsigned int pin;
} BasePinParameter;

typedef enum {
    BASE_PIN_MODE_INPUT,
    BASE_PIN_MODE_OUTPUT,
    BASE_PIN_MODE_INPUT_PULLUP
} BasePinMode;

typedef struct BasePinVtbl {
    void (*_doSetup)(BasePin *pThis, BasePinMode mode);
    unsigned int (*_doRead)(BasePin *pThis);
    void (*_doWrite)(BasePin *pThis, unsigned int value);
} BasePinVtbl;

// (De)constructors
PROTECTED void constructBasePin(BasePin *instance, BasePinParameter *parameter);
PROTECTED void deconstructBasePin(BasePin *instance);

// Public method(s)
PUBLIC void setupBasePin(BasePin *pThis, BasePinMode mode);
PUBLIC unsigned int readFromBasePin(BasePin *pThis);
PUBLIC void writeToBasePin(BasePin *pThis, unsigned int value);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __BASE_PIN_H__

