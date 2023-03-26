#include "port/common/inc/BaseFactory.h"

// Protected method(s)
PROTECTED void constructBaseFactory(BaseFactory *instance);
PROTECTED void deconstructBaseFactory(BaseFactory *instance);

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

PUBLIC void createBasePinByBaseFactory(
    BaseFactory *pThis, BasePin *instance, void *port, unsigned int pin)
{
    if (pThis != NULL) {
        pThis->vtbl->_doCreateBasePin(pThis, instance, port, pin);
    }
}

PUBLIC void destoryBasePinByBaseFactory(BaseFactory *pThis, BasePin *instance)
{
    if (pThis != NULL) {
        pThis->vtbl->_doDestoryBasePin(pThis, instance);
    }
}

