#ifndef __DATASELECTOR_H__
#define __DATASELECTOR_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stm32f10x.h"

#include "stdlib.h"

#include "keywords.h"

typedef struct DataSelector {
    uint8_t _startAddress, _endAddress;
    struct DataSelector * next;
} DataSelector;

// (de)constructor(s)
PUBLIC DataSelector newDataSelector(uint8_t startAddress, uint8_t endAddress);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DATASELECTOR_H__
