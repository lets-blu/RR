#include "basic/led/inc/LED.h"

// Private member(s)
PRIVATE STATIC LogFilter filter = STATIC_LOG_FILTER("LED", LOG_LEVEL_INFO);

// Method implement(s)
PUBLIC void constructLED(
    LED *instance, BasePinParameter *parameter, unsigned int lightState)
{
    DeviceManager *manager = instanceOfDeviceManager();

    if (instance != NULL) {
        instance->_pin = createPinByDeviceManager(
            manager, DEVICE_MANAGER_DIGITAL_PIN, parameter);

        instance->_lightState = lightState;
        setupBasePin(instance->_pin, BASE_PIN_MODE_OUTPUT);
    }
}

PUBLIC void deconstructLED(LED *instance)
{
    DeviceManager *manager = instanceOfDeviceManager();

    if (instance != NULL) {
        destoryPinByDeviceManager(
            manager, DEVICE_MANAGER_DIGITAL_PIN, instance->_pin);

        memset(instance, 0, sizeof(LED));
    }
}

PUBLIC void turnOnLED(LED *pThis)
{
    if (pThis == NULL) {
        return;
    }

    if (pThis->_lightState == BASE_PIN_STATE_LOW) {
        writeToBasePin(pThis->_pin, BASE_PIN_STATE_LOW);
    } else {
        writeToBasePin(pThis->_pin, BASE_PIN_STATE_HIGH);
    }

    LOG_I(&filter, "0x%x turned on", pThis);
}

PUBLIC void turnOffLED(LED *pThis)
{
    if (pThis == NULL) {
        return;
    }

    if (pThis->_lightState == BASE_PIN_STATE_LOW) {
        writeToBasePin(pThis->_pin, BASE_PIN_STATE_HIGH);
    } else {
        writeToBasePin(pThis->_pin, BASE_PIN_STATE_LOW);
    }

    LOG_I(&filter, "0x%x turned off", pThis);
}

