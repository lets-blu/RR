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
        instance->_pinPool = NULL;
    }
}

PUBLIC void deconstructDeviceManager(DeviceManager *instance)
{
    if (instance != NULL) {
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

PUBLIC void setPinPoolToDeviceManager(
    DeviceManager *pThis, DevicePool *pool)
{
    if (pThis != NULL) {
        pThis->_pinPool = pool;
    }
}

PUBLIC BasePin *createPinByDeviceManager(
    DeviceManager *pThis,
    DeviceManagerPinType type,
    BasePinParameter *parameter)
{
    BasePin *instance = NULL;

    if (pThis == NULL || parameter == NULL) {
        return NULL;
    }

    if (pThis->_pinPool == NULL || pThis->_factory == NULL) {
        return NULL;
    }

    instance = BaseDevice2BasePin(allocateFromDevicePool(pThis->_pinPool));

    if (instance != NULL) {
        createPinByBaseFactory(pThis->_factory, type, instance, parameter);
    }

    return instance;
}

PUBLIC void destoryPinByDeviceManager(
    DeviceManager *pThis, DeviceManagerPinType type, BasePin *instance)
{
    if (pThis == NULL || instance == NULL) {
        return;
    }

    if (pThis->_factory != NULL) {
        destoryPinByBaseFactory(pThis->_factory, type, instance);
    }

    if (pThis->_pinPool != NULL) {
        freeToDevicePool(pThis->_pinPool, &instance->base);
    }
}

PUBLIC STATIC DeviceManager *instanceOfDeviceManager(void)
{
    if (!instance._isConstructed) {
        constructDeviceManager(&instance);
    }

    return &instance;
}

// TODO: need to remove
PUBLIC BasePin *createBasePinByDeviceManager(
    DeviceManager *pThis, void *port, unsigned int pin)
{
    BasePin *instance = NULL;

    if (pThis == NULL || pThis->_factory == NULL) {
        return NULL;
    }

    instance = BaseDevice2BasePin(allocateFromDevicePool(pThis->_pinPool));

    if (instance == NULL) {
        return NULL;
    }

    createBasePinByBaseFactory(pThis->_factory, instance, port, pin);

    return instance;
}

// TODO: need to remove
PUBLIC void destoryBasePinByDeviceManager(
    DeviceManager *pThis, BasePin *instance)
{
    if (pThis == NULL || instance == NULL) {
        return;
    }

    if (pThis->_factory != NULL) {
        destoryBasePinByBaseFactory(pThis->_factory, instance);
    }

    freeToDevicePool(pThis->_pinPool, &instance->base);
}

