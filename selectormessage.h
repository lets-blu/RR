#ifndef __SELECTORMESSAGE_H__
#define __SELECTORMESSAGE_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stm32f10x.h"

#include "gpiopin.h"

typedef struct SelectorMessage {
    uint8_t id;
    GPIOPinState data;
} SelectorMessage;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SELECTORMESSAGE_H__
