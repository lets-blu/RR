#include "port/arduino/inc/ArduinoFactory.h"

// Override method(s)
PROTECTED OVERRIDE void doCreatePinByArduinoFactoryBase(
    BaseFactory *factory,
    BaseFactoryPinType type,
    BasePin *instance,
    BasePinParameter *parameter);

PROTECTED OVERRIDE void doDestoryPinByArduinoFactoryBase(
    BaseFactory *factory, BaseFactoryPinType type, BasePin *instance);

PROTECTED OVERRIDE unsigned int doGetPinSizeFromArduinoFactoryBase(
    BaseFactory *factory);

// Virtual methods table
static const BaseFactoryVtbl baseVtbl = {
    ._doCreatePin       = doCreatePinByArduinoFactoryBase,
    ._doDestoryPin      = doDestoryPinByArduinoFactoryBase,
    ._doGetPinSize      = doGetPinSizeFromArduinoFactoryBase
};

// Method implement(s)
PUBLIC void constructArduinoFactory(ArduinoFactory *instance)
{
    if (instance != NULL) {
        constructBaseFactory(&instance->base);
        instance->base.vtbl = &baseVtbl;
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
            constructArduinoAnalogPin(
                BasePin2ArduinoAnalogPin(instance), parameter);

            break;
        }

        case BASE_FACTORY_ADDRESS_PIN: {
            constructArduinoAddressPin(
                BasePin2ArduinoAddressPin(instance), parameter);

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
            deconstructArduinoAnalogPin(BasePin2ArduinoAnalogPin(instance));
            break;
        }

        case BASE_FACTORY_ADDRESS_PIN: {
            deconstructArduinoAddressPin(BasePin2ArduinoAddressPin(instance));
            break;
        }

        default: {
            break;
        }
    }
}

PROTECTED OVERRIDE unsigned int doGetPinSizeFromArduinoFactoryBase(
    BaseFactory *factory)
{
    (void)factory;
    return sizeof(union {ArduinoAddressPin a1; ArduinoAnalogPin a2; ArduinoDigitalPin d;});
}

