#include "core/device/inc/DeviceManager.h"

// Private member(s)
PRIVATE STATIC DeviceManager instance;

// Private method(s)
PRIVATE void constructDeviceManager(DeviceManager *instance);

// Method implement(s)
PRIVATE void constructDeviceManager(DeviceManager *instance)
{
    if (instance != NULL) {
        instance->_isConstructed = true;
        instance->_factory = NULL;
    }
}

PUBLIC void deconstructDeviceManager(DeviceManager *instance)
{
    if (instance != NULL) {
        deconstructDevicePool(&instance->_basePinPool);
        memset(instance, 0, sizeof(DeviceManager));
    }
}

PUBLIC void setFactoryToDeviceManager(
    DeviceManager *pThis, BaseFactory *factory)
{
    if (pThis != NULL) {
        pThis->_factory = factory;
    }
}

PUBLIC void setBasePinToDeviceManager(
    DeviceManager *pThis, unsigned int number, unsigned int size)
{
    if (pThis != NULL && !IS_DEVICE_POOL_CONSTRUCTED(&pThis->_basePinPool)) {
        constructDevicePool(&pThis->_basePinPool, number, size);
    }
}

PUBLIC BasePin *createBasePinByDeviceManager(
    DeviceManager *pThis, void *port, unsigned int pin)
{
    BasePin *instance = NULL;

    if (pThis == NULL || pThis->_factory == NULL) {
        return NULL;
    }

    instance = BaseDevice2BasePin(allocateFromDevicePool(&pThis->_basePinPool));

    if (instance == NULL) {
        return NULL;
    }

    createBasePinByBaseFactory(pThis->_factory, instance, port, pin);

    return instance;
}

PUBLIC void destoryBasePinByDeviceManager(
    DeviceManager *pThis, BasePin *instance)
{
    if (pThis == NULL || instance == NULL) {
        return;
    }

    if (pThis->_factory != NULL) {
        destoryBasePinByBaseFactory(pThis->_factory, instance);
    }

    freeToDevicePool(&pThis->_basePinPool, &instance->base);
}

PUBLIC STATIC DeviceManager *instanceOfDeviceManager(void)
{
    if (!IS_DEVICE_MANAGER_CONSTRUCTED(&instance)) {
        constructDeviceManager(&instance);
    }

    return &instance;
}

