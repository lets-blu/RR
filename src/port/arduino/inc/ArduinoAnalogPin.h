#ifndef __ARDUINO_ANALOG_PIN_H__
#define __ARDUINO_ANALOG_PIN_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "port/common/inc/BasePin.h"

#define BasePin2ArduinoAnalogPin(instance) \
    BASE_TO_SUB_CAST(instance, ArduinoAnalogPin, base)

typedef struct {
    BasePin base;
} ArduinoAnalogPin;

// (De)constructors
PUBLIC void constructArduinoAnalogPin(
    ArduinoAnalogPin *instance, BasePinParameter *parameter);

PUBLIC void deconstructArduinoAnalogPin(ArduinoAnalogPin *instance);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ARDUINO_ANALOG_PIN_H__

