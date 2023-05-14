#include "basic/led/inc/LED.h"

// Private member(s)
PRIVATE STATIC LogFilter filter = STATIC_LOG_FILTER("LED", LOG_LEVEL_INFO);

PUBLIC void constructLED(
    LED *instance, void *port, unsigned int pin, PinState lightState)
{
    DeviceManager *manager = instanceOfDeviceManager();

    if (instance != NULL) {
        instance->_basePin = createBasePinByDeviceManager(manager, port, pin);
        instance->_lightState = lightState;
        setupBasePin(instance->_basePin, PIN_MODE_OUTPUT);
    }
}

PUBLIC void deconstructLED(LED *instance)
{
    DeviceManager *manager = instanceOfDeviceManager();

    if (instance != NULL) {
        destoryBasePinByDeviceManager(manager, instance->_basePin);
        memset(instance, 0, sizeof(LED));
    }
}

PUBLIC void turnOnLED(LED *pThis)
{
    if (pThis == NULL) {
        return;
    }

    if (pThis->_lightState == PIN_STATE_LOW) {
        writeStateToBasePin(pThis->_basePin, PIN_STATE_LOW);
    } else {
        writeStateToBasePin(pThis->_basePin, PIN_STATE_HIGH);
    }

    LOG_I(&filter, "0x%x turned on", pThis);
}

PUBLIC void turnOffLED(LED *pThis)
{
    if (pThis == NULL) {
        return;
    }

    if (pThis->_lightState == PIN_STATE_LOW) {
        writeStateToBasePin(pThis->_basePin, PIN_STATE_HIGH);
    } else {
        writeStateToBasePin(pThis->_basePin, PIN_STATE_LOW);
    }

    LOG_I(&filter, "0x%x turned off", pThis);
}

