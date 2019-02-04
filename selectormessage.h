#ifndef __SELECTORMESSAGE_H__
#define __SELECTORMESSAGE_H__

#include "stm32f10x.h"

#include "gpiopin.h"
#include "keywords.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    typedef struct SelectorMessage {
        uint8_t _id;
        GPIOPinState _data;
    } SelectorMessage;

    PUBLIC SelectorMessage newSelectorMessage(uint8_t id, GPIOPinState data);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SELECTORMESSAGE_H__
