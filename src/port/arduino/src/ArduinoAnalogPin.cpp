#ifdef ARDUINO

#include <Arduino.h>
#include "port/arduino/inc/ArduinoAnalogPin.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Override method(s)
PROTECTED OVERRIDE void doSetupArduinoAnalogPinBase(
    BasePin *pin, BasePinMode mode);

PROTECTED OVERRIDE unsigned int doReadFromArduinoAnalogPinBase(
    BasePin *pin);

PROTECTED OVERRIDE void doWriteToArduinoAnalogPinBase(
    BasePin *pin, unsigned int value);

// Virtual methods table
static const BasePinVtbl baseVtbl = {
    ._doSetup       = doSetupArduinoAnalogPinBase,
    ._doRead        = doReadFromArduinoAnalogPinBase,
    ._doWrite       = doWriteToArduinoAnalogPinBase,

    // TODO: need to remove
    ._doReadState   = NULL,
    ._doWriteState  = NULL,
    ._doReadValue   = NULL,
    ._doWriteValue  = NULL
};

// Method implement(s)
PUBLIC void constructArduinoAnalogPin(
    ArduinoAnalogPin *instance, BasePinParameter *parameter)
{
    if (instance != NULL) {
        constructBasePin(&instance->base, parameter);
        instance->base.vtbl = &baseVtbl;
    }
}

PUBLIC void deconstructArduinoAnalogPin(ArduinoAnalogPin *instance)
{
    if (instance != NULL) {
        deconstructBasePin(&instance->base);
        memset(instance, 0, sizeof(ArduinoAnalogPin));
    }
}

PROTECTED OVERRIDE void doSetupArduinoAnalogPinBase(
    BasePin *pin, BasePinMode mode)
{
    ArduinoAnalogPin *pThis = BasePin2ArduinoAnalogPin(pin);

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

PROTECTED OVERRIDE unsigned int doReadFromArduinoAnalogPinBase(
    BasePin *pin)
{
    ArduinoAnalogPin *pThis = BasePin2ArduinoAnalogPin(pin);
    return analogRead(pThis->base._pin);
}

PROTECTED OVERRIDE void doWriteToArduinoAnalogPinBase(
    BasePin *pin, unsigned int value)
{
    ArduinoAnalogPin *pThis = BasePin2ArduinoAnalogPin(pin);
    analogWrite(pThis->base._pin, value);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ARDUINO

