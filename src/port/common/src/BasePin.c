#include "port/common/inc/BasePin.h"

// Protected method(s)
PROTECTED void constructBaseDevice(BaseDevice *instance);
PROTECTED void deconstructBaseDevice(BaseDevice *instance);

// Method implement(s)
PROTECTED void constructBasePin(BasePin *instance, BasePinParameter *parameter)
{
    if (instance == NULL || parameter == NULL) {
        return;
    }

    // 1. Construct base
    constructBaseDevice(&instance->base);

    // 2. Initialize member(s)
    instance->_port = parameter->port;
    instance->_pin = parameter->pin;
    instance->vtbl = NULL;
}

PROTECTED void deconstructBasePin(BasePin *instance)
{
    if (instance != NULL) {
        deconstructBaseDevice(&instance->base);
        memset(instance, 0, sizeof(BasePin));
    }
}

PUBLIC void setupBasePin(BasePin *pThis, BasePinMode mode)
{
    if (pThis != NULL) {
        pThis->vtbl->_doSetup(pThis, mode);
    }
}

PUBLIC unsigned int readFromBasePin(BasePin *pThis)
{
    if (pThis == NULL) {
        return 0;
    }

    return pThis->vtbl->_doRead(pThis);
}

PUBLIC void writeToBasePin(BasePin *pThis, unsigned int value)
{
    if (pThis != NULL) {
        pThis->vtbl->_doWrite(pThis, value);
    }
}

