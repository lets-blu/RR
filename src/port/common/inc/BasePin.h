#ifndef __BASE_PIN_H__
#define __BASE_PIN_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "core/device/inc/BaseDevice.h"

#define BaseDevice2BasePin(instance) \
    BASE_TO_SUB_CAST(instance, BasePin, base)

struct BasePinVtbl;

typedef enum {
    PIN_MODE_INPUT,
    PIN_MODE_OUTPUT,
    PIN_MODE_INPUT_PULLUP
} PinMode;

typedef enum {
    PIN_STATE_LOW,
    PIN_STATE_HIGH
} PinState;

typedef struct {
    BaseDevice base;

    void *_port;
    unsigned int _pin;
    const struct BasePinVtbl *vtbl;
} BasePin;

typedef struct BasePinVtbl {
    void (*_doSetup)(BasePin *pThis, PinMode mode);

    PinState (*_doReadState)(BasePin *pThis);
    void (*_doWriteState)(BasePin *pThis, PinState state);

    unsigned int (*_doReadValue)(BasePin *pThis);
    void (*_doWriteValue)(BasePin *pThis, unsigned int value);
} BasePinVtbl;

// Public method(s)
PUBLIC void setupBasePin(BasePin *pThis, PinMode mode);

PUBLIC PinState readStateFromBasePin(BasePin *pThis);
PUBLIC void writeStateToBasePin(BasePin *pThis, PinState state);

PUBLIC unsigned int readValueFromBasePin(BasePin *pThis);
PUBLIC void writeValueToBasePin(BasePin *pThis, unsigned int value);

PUBLIC void *getPortFromBasePin(BasePin *pThis);
PUBLIC unsigned int getPinFromBasePin(BasePin *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __BASE_PIN_H__

