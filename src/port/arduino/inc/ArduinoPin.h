#ifndef __ARDUINO_PIN_H__
#define __ARDUINO_PIN_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "port/common/inc/BasePin.h"

#define BasePin2ArduinoPin(instance) \
    BASE_TO_SUB_CAST(instance, ArduinoPin, base)

typedef struct {
    BasePin base;
} ArduinoPin;

// (De)constructors
PUBLIC void constructArduinoPin(ArduinoPin *instance, uint8_t pin);
PUBLIC void deconstructArduinoPin(ArduinoPin *instance);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ARDUINO_PIN_H__

