#ifdef ARDUINO

#include <Arduino.h>
#include "port/arduino/inc/ArduinoDigitalPin.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Override method(s)
PROTECTED OVERRIDE void doSetupArduinoDigitalPinBase(
    BasePin *pin, BasePinMode mode);

PROTECTED OVERRIDE unsigned int doReadFromArduinoDigitalPinBase(
    BasePin *pin);

PROTECTED OVERRIDE void doWriteToArduinoDigitalPinBase(
    BasePin *pin, unsigned int value);

// Virtual methods table
static const BasePinVtbl baseVtbl = {
    ._doSetup       = doSetupArduinoDigitalPinBase,
    ._doRead        = doReadFromArduinoDigitalPinBase,
    ._doWrite       = doWriteToArduinoDigitalPinBase
};

// Method implement(s)
PUBLIC void constructArduinoDigitalPin(
    ArduinoDigitalPin *instance, BasePinParameter *parameter)
{
    if (instance != NULL) {
        constructBasePin(&instance->base, parameter);
        instance->base.vtbl = &baseVtbl;
    }
}

PUBLIC void deconstructArduinoDigitalPin(ArduinoDigitalPin *instance)
{
    if (instance != NULL) {
        deconstructBasePin(&instance->base);
        memset(instance, 0, sizeof(ArduinoDigitalPin));
    }
}

PROTECTED OVERRIDE void doSetupArduinoDigitalPinBase(
    BasePin *pin, BasePinMode mode)
{
    ArduinoDigitalPin *pThis = NULL;

    if (pin == NULL) {
        return;
    }

    pThis = BasePin2ArduinoDigitalPin(pin);

    switch (mode) {
        case BASE_PIN_MODE_INPUT: {
            pinMode(pThis->base._pin, INPUT);
            break;
        }

        case BASE_PIN_MODE_OUTPUT: {
            pinMode(pThis->base._pin, OUTPUT);
            break;
        }

        case BASE_PIN_MODE_INPUT_PULLUP: {
            pinMode(pThis->base._pin, INPUT_PULLUP);
            break;
        }

        default: {
            break;
        }
    }
}

PROTECTED OVERRIDE unsigned int doReadFromArduinoDigitalPinBase(
    BasePin *pin)
{
    ArduinoDigitalPin *pThis = NULL;

    if (pin == NULL) {
        return 0;
    }

    pThis = BasePin2ArduinoDigitalPin(pin);

    if (digitalRead(pThis->base._pin) == LOW) {
        return BASE_PIN_VALUE_LOW;
    } else {
        return BASE_PIN_VALUE_HIGH;
    }
}

PROTECTED OVERRIDE void doWriteToArduinoDigitalPinBase(
    BasePin *pin, unsigned int value)
{
    ArduinoDigitalPin *pThis = NULL;

    if (pin == NULL) {
        return;
    }

    pThis = BasePin2ArduinoDigitalPin(pin);

    if (value == BASE_PIN_VALUE_LOW) {
        digitalWrite(pThis->base._pin, LOW);
    } else {
        digitalWrite(pThis->base._pin, HIGH);
    }
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ARDUINO

