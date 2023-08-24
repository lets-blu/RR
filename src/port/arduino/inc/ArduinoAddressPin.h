#ifndef __ARDUINO_ADDRESS_PIN_H__
#define __ARDUINO_ADDRESS_PIN_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "port/common/inc/BasePin.h"

#define BasePin2ArduinoAddressPin(instance) \
    BASE_TO_SUB_CAST(instance, ArduinoAddressPin, base)

typedef struct {
    BasePin base;
} ArduinoAddressPin;

// (De)constructors
PUBLIC void constructArduinoAddressPin(
    ArduinoAddressPin *instance, BasePinParameter *parameter);

PUBLIC void deconstructArduinoAddressPin(ArduinoAddressPin *instance);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ARDUINO_ADDRESS_PIN_H__

