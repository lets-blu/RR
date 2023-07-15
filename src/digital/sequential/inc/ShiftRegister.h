#ifndef __SHIFT_REGISTER_H__
#define __SHIFT_REGISTER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "core/list/inc/LinkedList.h"

#define LinkedListItem2ShiftRegister(instance) \
    BASE_TO_SUB_CAST(instance, ShiftRegister, base)

typedef struct {
    LinkedListItem base;
    unsigned int _data;
    unsigned int _dataSize;
} ShiftRegister;

// (De)constructors
PUBLIC void constructShiftRegister(
    ShiftRegister *instance, unsigned int dataSize);

PUBLIC void deconstructShiftRegister(ShiftRegister *instance);

// Public method(s)
PUBLIC void setBitToShiftRegister(ShiftRegister *pThis, unsigned int bit);
PUBLIC void resetBitFromShiftRegister(ShiftRegister *pThis, unsigned int bit);

PUBLIC unsigned int getDataFromShiftRegister(ShiftRegister *pThis);
PUBLIC unsigned int getDataSizeFromShiftRegister(ShiftRegister *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SHIFT_REGISTER_H__

