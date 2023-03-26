#include "port/arduino/inc/ArduinoFactory.h"

// Protected method(s)
PROTECTED void constructBaseFactory(BaseFactory *instance);
PROTECTED void deconstructBaseFactory(BaseFactory *instance);

PROTECTED void createBasePinByArduinoFactory(
    ArduinoFactory *pThis, BasePin *instance, void *port, unsigned int pin);

PROTECTED void destoryBasePinByArduinoFactory(
    ArduinoFactory *pThis, BasePin *instance);

// Virtual methods table
static const BaseFactoryVtbl factoryVtbl ={
    ._doCreateBasePin
        = (BaseFactoryDoCreateBasePinMethod)createBasePinByArduinoFactory,

    ._doDestoryBasePin
        = (BaseFactoryDoDestoryBasePinMethod)destoryBasePinByArduinoFactory
};

// Method implement(s)
PUBLIC void constructArduinoFactory(ArduinoFactory *instance)
{
    if (instance != NULL) {
        constructBaseFactory(&instance->base);
        instance->base.vtbl = &factoryVtbl;
    }
}

PUBLIC void deconstructAduinoFactory(ArduinoFactory *instance)
{
    if (instance != NULL) {
        deconstructBaseFactory(&instance->base);
        memset(instance, 0, sizeof(ArduinoFactory));
    }
}

PROTECTED void createBasePinByArduinoFactory(
    ArduinoFactory *pThis, BasePin *instance, void *port, unsigned int pin)
{
    (void)port;

    if (pThis != NULL) {
        constructArduinoPin(BasePin2ArduinoPin(instance), (uint8_t)pin);
    }
}

PROTECTED void destoryBasePinByArduinoFactory(
    ArduinoFactory *pThis, BasePin *instance)
{
    if (pThis != NULL) {
        deconstructArduinoPin(BasePin2ArduinoPin(instance));
    }
}

