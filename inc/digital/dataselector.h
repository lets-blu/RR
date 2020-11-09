#ifndef __DATASELECTOR_H__
#define __DATASELECTOR_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "assert.h"
#include "stddef.h"
#include "stdint.h"

#include "keywords.h"

typedef struct DataSelector {
    uint8_t _startAddress;
    uint8_t _endAddress;
    struct DataSelector * _next;
} DataSelector;

// Constructor
PUBLIC DataSelector newDataSelector(uint8_t startAddress, uint8_t endAddress);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DATASELECTOR_H__
