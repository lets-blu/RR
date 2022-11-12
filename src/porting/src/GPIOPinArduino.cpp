#ifdef ARDUINO

#include <Arduino.h>
#include "porting/inc/GPIOPin.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

PUBLIC void constructGPIOPin(GPIOPin *instance, GPIOPinPort port, GPIOPinPin pin)
{
    instance->_port = port;
    instance->_pin = pin;
}

PUBLIC void deconstructGPIOPin(GPIOPin *instance)
{
    memset(instance, 0, sizeof(GPIOPin));
}

PUBLIC void setupGPIOPin(GPIOPin *pThis, GPIOPinMode mode)
{
    if (pThis != NULL)
    {
        pinMode(pThis->_pin, mode);
    }
}

PUBLIC GPIOPinState readFromGPIOPin(GPIOPin *pThis)
{
    if (pThis != NULL)
    {
        return digitalRead(pThis->_pin);
    }

    return LOW;
}

PUBLIC void writeToGPIOPin(GPIOPin *pThis, GPIOPinState state)
{
    if (pThis != NULL)
    {
        digitalWrite(pThis->_pin, state);
    }
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ARDUINO
