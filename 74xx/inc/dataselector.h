#ifndef __DATASELECTOR_H__
#define __DATASELECTOR_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdint.h"
#include "stdlib.h"

#include "keywords.h"

typedef struct DataSelector
{
    uint8_t _startAddress;
    uint8_t _endAddress;
    
    struct DataSelector * next;
} DataSelector;

// constructor
PUBLIC DataSelector newDataSelector(uint8_t startAddress, uint8_t endAddress);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DATASELECTOR_H__
