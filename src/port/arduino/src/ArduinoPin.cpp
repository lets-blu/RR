#ifdef ARDUINO

#include <Arduino.h>
#include "port/arduino/inc/ArduinoPin.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Protected method(s)
PROTECTED void constructBasePin(BasePin *instance, void *port, unsigned int pin);
PROTECTED void deconstructBasePin(BasePin *instance);

PROTECTED void doSetupArduinoPin(ArduinoPin *pThis, PinMode mode);

PROTECTED PinState doReadStateFromArduinoPin(ArduinoPin *pThis);
PROTECTED void doWriteStateToArduinoPin(ArduinoPin *pThis, PinState state);

PROTECTED unsigned int doReadValueFromArduinoPin(ArduinoPin *pThis);
PROTECTED void doWriteValueToArduinoPin(ArduinoPin *pThis, unsigned int value);

// Virtial methods table
static const BasePinVtbl basePinVtbl = {
    ._doSetup       = (BasePinDoSetupMethod)doSetupArduinoPin,

    ._doReadState   = (BasePinDoReadStateMethod)doReadStateFromArduinoPin,
    ._doWriteState  = (BasePinDoWriteStateMethod)doWriteStateToArduinoPin,

    ._doReadValue   = (BasePinDoReadValueMethod)doReadValueFromArduinoPin,
    ._doWriteValue  = (BasePinDoWriteValueMethod)doWriteValueToArduinoPin
};

PUBLIC void constructArduinoPin(ArduinoPin *instance, uint8_t pin)
{
    if (instance != NULL) {
        constructBasePin(&instance->base, NULL, pin);
        instance->base.vtbl = &basePinVtbl;
    }
}

PUBLIC void deconstructArduinoPin(ArduinoPin *instance)
{
    if (instance != NULL) {
        deconstructBasePin(&instance->base);
        memset(instance, 0, sizeof(ArduinoPin));
    }
}

PROTECTED void doSetupArduinoPin(ArduinoPin *pThis, PinMode mode)
{
    if (pThis == NULL) {
        return;
    }

    switch (mode) {
        case PIN_MODE_INPUT: {
            pinMode(getPinFromBasePin(&pThis->base), mode);
            break;
        }

        case PIN_MODE_OUTPUT: {
            pinMode(getPinFromBasePin(&pThis->base), mode);
            break;
        }

        case PIN_MODE_INPUT_PULLUP: {
            pinMode(getPinFromBasePin(&pThis->base), mode);
            break;
        }

        default: {
            break;
        }
    }
}

PROTECTED PinState doReadStateFromArduinoPin(ArduinoPin *pThis)
{
    if (pThis == NULL) {
        return PIN_STATE_LOW;
    }

    if (digitalRead(getPinFromBasePin(&pThis->base)) == LOW) {
        return PIN_STATE_LOW;
    } else {
        return PIN_STATE_HIGH;
    }
}

PROTECTED void doWriteStateToArduinoPin(ArduinoPin *pThis, PinState state)
{
    if (pThis == NULL) {
        return;
    }

    if (state == PIN_STATE_LOW) {
        digitalWrite(getPinFromBasePin(&pThis->base), LOW);
    } else {
        digitalWrite(getPinFromBasePin(&pThis->base), HIGH);
    }
}

PROTECTED unsigned int doReadValueFromArduinoPin(ArduinoPin *pThis)
{
    if (pThis != NULL) {
        return analogRead(getPinFromBasePin(&pThis->base));
    }

    return 0;
}

PROTECTED void doWriteValueToArduinoPin(ArduinoPin *pThis, unsigned int value)
{
    if (pThis != NULL) {
        analogWrite(getPinFromBasePin(&pThis->base), value);
    }
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ARDUINO

