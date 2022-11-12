#ifndef __GPIO_PIN_ARDUINO_H__
#define __GPIO_PIN_ARDUINO_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define GENERIC_ARDUINO_PORT    NULL
typedef void *GPIOPinPort;

#ifndef INPUT
#define INPUT                   0x0
#endif
#ifndef OUTPUT
#define OUTPUT                  0x1
#endif
#ifndef INPUT_PULLUP
#define INPUT_PULLUP            0x2
#endif
typedef uint8_t GPIOPinMode;

#ifndef LOW
#define LOW                     0x0
#endif
#ifndef HIGH
#define HIGH                    0x1
#endif
typedef int GPIOPinState;

typedef uint8_t GPIOPinPin;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __GPIO_PIN_ARDUINO_H__
