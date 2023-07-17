#include "port/arduino/inc/ArduinoFactory.h"

// Override method(s)
PROTECTED OVERRIDE void doCreatePinByArduinoFactoryBase(
    BaseFactory *factory,
    BaseFactoryPinType type,
    BasePin *instance,
    BasePinParameter *parameter);

PROTECTED OVERRIDE void doDestoryPinByArduinoFactoryBase(
    BaseFactory *factory, BaseFactoryPinType type, BasePin *instance);

// TODO: need to remove
PROTECTED OVERRIDE void doCreateBasePinByArduinoFactoryBase(
    BaseFactory *factory, BasePin *instance, void *port, unsigned int pin);

// TODO: need to remove
PROTECTED OVERRIDE void doDestoryBasePinByArduinoFactoryBase(
    BaseFactory *factory, BasePin *instance);

// Virtual methods table
static const BaseFactoryVtbl factoryVtbl = {
    ._doCreatePin       = doCreatePinByArduinoFactoryBase,
    ._doDestoryPin      = doDestoryPinByArduinoFactoryBase,

    // TODO: need to remove
    ._doCreateBasePin   = doCreateBasePinByArduinoFactoryBase,
    ._doDestoryBasePin  = doDestoryBasePinByArduinoFactoryBase,

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

PROTECTED OVERRIDE void doCreatePinByArduinoFactoryBase(
    BaseFactory *factory,
    BaseFactoryPinType type,
    BasePin *instance,
    BasePinParameter *parameter)
{
    (void)factory;

    switch (type) {
        case BASE_FACTORY_DIGITAL_PIN: {
            constructArduinoDigitalPin(
                BasePin2ArduinoDigitalPin(instance), parameter);

            break;
        }

        case BASE_FACTORY_ANALOG_PIN: {
            break;
        }

        case BASE_FACTORY_ADDRESS_PIN: {
            break;
        }

        default: {
            break;
        }
    }
}

PROTECTED OVERRIDE void doDestoryPinByArduinoFactoryBase(
    BaseFactory *factory, BaseFactoryPinType type, BasePin *instance)
{
    (void)factory;

    switch (type) {
        case BASE_FACTORY_DIGITAL_PIN: {
            deconstructArduinoDigitalPin(BasePin2ArduinoDigitalPin(instance));
            break;
        }

        case BASE_FACTORY_ANALOG_PIN: {
            break;
        }

        case BASE_FACTORY_ADDRESS_PIN: {
            break;
        }

        default: {
            break;
        }
    }
}

// TODO: need to remove
PROTECTED OVERRIDE void doCreateBasePinByArduinoFactoryBase(
    BaseFactory *factory, BasePin *instance, void *port, unsigned int pin)
{
    (void)port;

    if (factory != NULL && instance != NULL) {
        constructArduinoPin(BasePin2ArduinoPin(instance), (uint8_t)pin);
    }
}

// TODO: need to remove
PROTECTED OVERRIDE void doDestoryBasePinByArduinoFactoryBase(
    BaseFactory *factory, BasePin *instance)
{
    if (factory != NULL && instance != NULL) {
        deconstructArduinoPin(BasePin2ArduinoPin(instance));
    }
}

