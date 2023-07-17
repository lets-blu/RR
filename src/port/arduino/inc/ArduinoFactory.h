#ifndef __ARDUINO_FACTORY_H__
#define __ARDUINO_FACTORY_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"

#include "port/arduino/inc/ArduinoPin.h"
#include "port/arduino/inc/ArduinoDigitalPin.h"
#include "port/common/inc/BasePin.h"
#include "port/common/inc/BaseFactory.h"

#define BaseFactory2ArduinoFactory(instance) \
    BASE_TO_SUB_CAST(instance, ArduinoFactory, base)

typedef struct {
    BaseFactory base;
} ArduinoFactory;

// (De)constructors
PUBLIC void constructArduinoFactory(ArduinoFactory *instance);
PUBLIC void deconstructAduinoFactory(ArduinoFactory *instance);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ARDUINO_FACTORY_H__

