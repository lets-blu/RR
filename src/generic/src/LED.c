#include "generic/inc/LED.h"

// Private member(s)
PRIVATE STATIC LogFilter filter = STATIC_LOG_FILTER("LED", LOG_LEVEL_INFO);

PUBLIC void constructLED(LED *instance, GPIOPin *pin, GPIOPinState lightState)
{
    instance->_lightState = lightState;
    memcpy(&instance->_pin, pin, sizeof(GPIOPin));

    setupGPIOPin(pin, OUTPUT);
    turnOffLED(instance);
}

PUBLIC void deconstructLED(LED *instance)
{
    memset(instance, 0, sizeof(LED));
}

PUBLIC void turnOnLED(LED *pThis)
{
    if (pThis != NULL)
    {
        writeToGPIOPin(&pThis->_pin, pThis->_lightState);
        LOG_I(&filter, "0x%x turned on", pThis);
    }
}

PUBLIC void turnOffLED(LED *pThis)
{
    if (pThis != NULL)
    {
        writeToGPIOPin(&pThis->_pin, !pThis->_lightState);
        LOG_I(&filter, "0x%x turned off", pThis);
    }
}
