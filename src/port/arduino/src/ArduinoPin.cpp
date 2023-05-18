#ifdef ARDUINO

#include <Arduino.h>
#include "port/arduino/inc/ArduinoPin.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Protected method(s)
PROTECTED void constructBasePin(BasePin *instance, void *port, unsigned int pin);
PROTECTED void deconstructBasePin(BasePin *instance);

// Override method(s)
PROTECTED OVERRIDE void doSetupArduinoPinBase(
    BasePin *pin, PinMode mode);

PROTECTED OVERRIDE PinState doReadStateFromArduinoPinBase(
    BasePin *pin);

PROTECTED OVERRIDE void doWriteStateToArduinoPinBase(
    BasePin *pin, PinState state);

PROTECTED OVERRIDE unsigned int doReadValueFromArduinoPinBase(
    BasePin *pin);

PROTECTED OVERRIDE void doWriteValueToArduinoPinBase(
    BasePin *pin, unsigned int value);

// Virtial methods table
static const BasePinVtbl basePinVtbl = {
    ._doSetup       = doSetupArduinoPinBase,
    
    ._doReadState   = doReadStateFromArduinoPinBase,
    ._doWriteState  = doWriteStateToArduinoPinBase,

    ._doReadValue   = doReadValueFromArduinoPinBase,
    ._doWriteValue  = doWriteValueToArduinoPinBase
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

PROTECTED OVERRIDE void doSetupArduinoPinBase(
    BasePin *pin, PinMode mode)
{
    if (pin == NULL) {
        return;
    }

    switch (mode) {
        case PIN_MODE_INPUT: {
            pinMode(getPinFromBasePin(pin), mode);
            break;
        }

        case PIN_MODE_OUTPUT: {
            pinMode(getPinFromBasePin(pin), mode);
            break;
        }

        case PIN_MODE_INPUT_PULLUP: {
            pinMode(getPinFromBasePin(pin), mode);
            break;
        }

        default: {
            break;
        }
    }
}

PROTECTED OVERRIDE PinState doReadStateFromArduinoPinBase(
    BasePin *pin)
{
    if (pin == NULL) {
        return PIN_STATE_LOW;
    }

    if (digitalRead(getPinFromBasePin(pin)) == LOW) {
        return PIN_STATE_LOW;
    } else {
        return PIN_STATE_HIGH;
    }
}

PROTECTED OVERRIDE void doWriteStateToArduinoPinBase(
    BasePin *pin, PinState state)
{
    if (pin == NULL) {
        return;
    }

    if (state == PIN_STATE_LOW) {
        digitalWrite(getPinFromBasePin(pin), LOW);
    } else {
        digitalWrite(getPinFromBasePin(pin), HIGH);
    }
}

PROTECTED OVERRIDE unsigned int doReadValueFromArduinoPinBase(
    BasePin *pin)
{
    if (pin != NULL) {
        return 0;
    }

    return analogRead(getPinFromBasePin(pin));
}

PROTECTED OVERRIDE void doWriteValueToArduinoPinBase(
    BasePin *pin, unsigned int value)
{
    if (pin != NULL) {
        analogWrite(getPinFromBasePin(pin), value);
    }
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ARDUINO

