#ifndef __SELECTORMESSAGE_H__
#define __SELECTORMESSAGE_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdint.h"

#include "gpiopin.h"

typedef struct {
    uint8_t address;
    GPIOPinState state;
} SelectorMessage;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SELECTORMESSAGE_H__
