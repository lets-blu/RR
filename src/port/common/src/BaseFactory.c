#include "port/common/inc/BaseFactory.h"

// Method implement(s)
PROTECTED void constructBaseFactory(BaseFactory *instance)
{
    if (instance != NULL) {
        instance->vtbl = NULL;
    }
}

PROTECTED void deconstructBaseFactory(BaseFactory *instance)
{
    if (instance != NULL) {
        memset(instance, 0, sizeof(BaseFactory));
    }
}

PUBLIC void createPinByBaseFactory(
    BaseFactory *pThis,
    BaseFactoryPinType type,
    BasePin *instance,
    BasePinParameter *parameter)
{
    if (pThis != NULL && instance != NULL && parameter != NULL) {
        pThis->vtbl->_doCreatePin(pThis, type, instance, parameter);
    }
}

PUBLIC void destoryPinByBaseFactory(
    BaseFactory *pThis, BaseFactoryPinType type, BasePin *instance)
{
    if (pThis != NULL && instance != NULL) {
        pThis->vtbl->_doDestoryPin(pThis, type, instance);
    }
}

PUBLIC unsigned int getPinSizeFromBaseFactory(BaseFactory *pThis)
{
    return (pThis == NULL) ? 0 : pThis->vtbl->_doGetPinSize(pThis);
}

