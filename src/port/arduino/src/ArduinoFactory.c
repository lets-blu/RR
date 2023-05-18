#include "port/arduino/inc/ArduinoFactory.h"

// Protected method(s)
PROTECTED void constructBaseFactory(BaseFactory *instance);
PROTECTED void deconstructBaseFactory(BaseFactory *instance);

// Override method(s)
PROTECTED OVERRIDE void doCreateBasePinByArduinoFactoryBase(
    BaseFactory *factory, BasePin *instance, void *port, unsigned int pin);

PROTECTED OVERRIDE void doDestoryBasePinByArduinoFactoryBase(
    BaseFactory *factory, BasePin *instance);

// Virtual methods table
static const BaseFactoryVtbl factoryVtbl = {
    ._doCreateBasePin   = doCreateBasePinByArduinoFactoryBase,
    ._doDestoryBasePin  = doDestoryBasePinByArduinoFactoryBase
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

PROTECTED OVERRIDE void doCreateBasePinByArduinoFactoryBase(
    BaseFactory *factory, BasePin *instance, void *port, unsigned int pin)
{
    (void)port;

    if (factory != NULL && instance != NULL) {
        constructArduinoPin(BasePin2ArduinoPin(instance), (uint8_t)pin);
    }
}

PROTECTED OVERRIDE void doDestoryBasePinByArduinoFactoryBase(
    BaseFactory *factory, BasePin *instance)
{
    if (factory != NULL && instance != NULL) {
        deconstructArduinoPin(BasePin2ArduinoPin(instance));
    }
}

