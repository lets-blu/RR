#include "port/common/inc/BasePin.h"

// Protected method(s)
PROTECTED void constructBaseDevice(BaseDevice *instance);
PROTECTED void deconstructBaseDevice(BaseDevice *instance);

PROTECTED void constructBasePin(BasePin *instance, void *port, unsigned int pin);
PROTECTED void deconstructBasePin(BasePin *instance);

// Method implement(s)
PROTECTED void constructBasePin(BasePin *instance, void *port, unsigned int pin)
{
    if (instance == NULL) {
        return;
    }

    // 1. Construct base
    constructBaseDevice(&instance->base);

    // 2. Initialize member(s)
    instance->_port = port;
    instance->_pin = pin;
    instance->vtbl = NULL;
}

PROTECTED void deconstructBasePin(BasePin *instance)
{
    if (instance != NULL) {
        deconstructBaseDevice(&instance->base);
        memset(instance, 0, sizeof(BasePin));
    }
}

PUBLIC void setupBasePin(BasePin *pThis, PinMode mode)
{
    if (pThis != NULL) {
        pThis->vtbl->_doSetup(pThis, mode);
    }
}

PUBLIC PinState readStateFromBasePin(BasePin *pThis)
{
    if (pThis != NULL) {
        return pThis->vtbl->_doReadState(pThis);
    }

    return PIN_STATE_LOW;
}

PUBLIC void writeStateToBasePin(BasePin *pThis, PinState state)
{
    if (pThis != NULL) {
        pThis->vtbl->_doWriteState(pThis, state);
    }
}

PUBLIC unsigned int readValueFromBasePin(BasePin *pThis)
{
    if (pThis != NULL) {
        return  pThis->vtbl->_doReadValue(pThis);
    }

    return 0;
}

PUBLIC void writeValueToBasePin(BasePin *pThis, unsigned int value)
{
    if (pThis != NULL) {
        pThis->vtbl->_doWriteValue(pThis, value);
    }
}

PUBLIC void *getPortFromBasePin(BasePin *pThis)
{
    if (pThis != NULL) {
        return pThis->_port;
    }

    return NULL;
}

PUBLIC unsigned int getPinFromBasePin(BasePin *pThis)
{
    if (pThis != NULL) {
        return pThis->_pin;
    }

    return 0;
}
