#ifndef __SELECTORMESSAGE_H__
#define __SELECTORMESSAGE_H__

#include "stm32f10x.h"

#include "gpiopin.h"
#include "keyword.h"

typedef struct SelectorMessage {
    uint8_t id;
    GPIOPinState data;
} SelectorMessage;

PUBLIC SelectorMessage newSelectorMessage(uint8_t id, GPIOPinState data);

#endif // __SELECTORMESSAGE_H__
