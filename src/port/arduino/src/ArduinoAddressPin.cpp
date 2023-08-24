#ifdef ARDUINO

#include <Arduino.h>
#include "port/arduino/inc/ArduinoAddressPin.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// Private method(s)
PRIVATE unsigned int offsetOfArduinoAddressPin(
    ArduinoAddressPin *pThis);

// Override method(s)
PROTECTED OVERRIDE void doSetupArduinoAddressPinBase(
    BasePin *pin, BasePinMode mode);

PROTECTED OVERRIDE unsigned int doReadFromArduinoAddressPinBase(
    BasePin *pin);

PROTECTED OVERRIDE void doWriteToArduinoAddressPinBase(
    BasePin *pin, unsigned int value);

// Virtual methods table
static const BasePinVtbl baseVtbl = {
    ._doSetup   = doSetupArduinoAddressPinBase,
    ._doRead    = doReadFromArduinoAddressPinBase,
    ._doWrite   = doWriteToArduinoAddressPinBase
};

// Method implements
PUBLIC void constructArduinoAddressPin(
    ArduinoAddressPin *instance, BasePinParameter *parameter)
{
    if (instance != NULL) {
        constructBasePin(&instance->base, parameter);
        instance->base.vtbl = &baseVtbl;
    }
}

PUBLIC void deconstructArduinoAddressPin(ArduinoAddressPin *instance)
{
    if (instance != NULL) {
        deconstructBasePin(&instance->base);
        memset(instance, 0, sizeof(ArduinoAddressPin));
    }
}

PRIVATE unsigned int offsetOfArduinoAddressPin(
    ArduinoAddressPin *pThis)
{
    unsigned int offset = 0;
    unsigned int pin = pThis->base._pin;

    while (pin != 0) {
        if ((pin & 1) != 0) {
            break;
        }

        offset++;
        pin >>= 1;
    }

    return offset;
}

PROTECTED OVERRIDE void doSetupArduinoAddressPinBase(
    BasePin *pin, BasePinMode mode)
{
    ArduinoAddressPin *pThis = NULL;

    if (pin == NULL) {
        return;
    }

    pThis = BasePin2ArduinoAddressPin(pin);

    switch (mode) {
        case BASE_PIN_MODE_INPUT:
        case BASE_PIN_MODE_INPUT_PULLUP: {
#if defined(ARDUINO_ARCH_AVR)
            if (pThis->base._port == &PORTB) {
                DDRB &= pThis->base._pin;
            } else if (pThis->base._port == &PORTC) {
                DDRC &= ~pThis->base._pin;
            } else if (pThis->base._port == &PORTD) {
                DDRD &= ~pThis->base._pin;
            } else {
                ;
            }
#endif
            break;
        }

        case BASE_PIN_MODE_OUTPUT: {
#if defined(ARDUINO_ARCH_AVR)
            if (pThis->base._port == &PORTB) {
                DDRB |= pThis->base._pin;
            } else if (pThis->base._port == &PORTC) {
                DDRC |= pThis->base._pin;
            } else if (pThis->base._port == &PORTD) {
                DDRD |= pThis->base._pin;
            } else {
                ;
            }
#endif
            break;
        }

        default: {
            break;
        }
    }
}

PROTECTED OVERRIDE unsigned int doReadFromArduinoAddressPinBase(
    BasePin *pin)
{
    unsigned int value = 0;
    ArduinoAddressPin *pThis = NULL;

    if (pin == NULL) {
        return 0;
    }

    pThis = BasePin2ArduinoAddressPin(pin);

#if defined(ARDUINO_ARCH_AVR)
    if (pThis->base._port == &PORTB) {
        value = PINB;
    } else if (pThis->base._port == &PORTC) {
        value = PINC;
    } else if (pThis->base._port == &PORTD) {
        value = PIND;
    } else {
        ;
    }
#endif

    value &= pThis->base._pin;
    value >>= offsetOfArduinoAddressPin(pThis);

    return value;
}

PROTECTED OVERRIDE void doWriteToArduinoAddressPinBase(
    BasePin *pin, unsigned int value)
{
    ArduinoAddressPin *pThis = NULL;

    if (pin == NULL) {
        return;
    }

    pThis = BasePin2ArduinoAddressPin(pin);

#if defined(ARDUINO_ARCH_AVR)
    *((uint8_t *)pThis->base._port) &= ~pThis->base._pin;
    *((uint8_t *)pThis->base._port) |= (value << offsetOfArduinoAddressPin(pThis));
#endif
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ARDUINO

