#ifndef __ARDUINO_DIGITAL_PIN_H__
#define __ARDUINO_DIGITAL_PIN_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "port/common/inc/BasePin.h"

#define BasePin2ArduinoDigitalPin(instance) \
    BASE_TO_SUB_CAST(instance, ArduinoDigitalPin, base)

typedef struct {
    BasePin base;
} ArduinoDigitalPin;

// (De)constructors
PUBLIC void constructArduinoDigitalPin(
    ArduinoDigitalPin *instance, BasePinParameter *parameter);

PUBLIC void deconstructArduinoDigitalPin(ArduinoDigitalPin *instance);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ARDUINO_DIGITAL_PIN_H__

