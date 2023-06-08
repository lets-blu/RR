#ifndef __SHIFT_REGISTER_H__
#define __SHIFT_REGISTER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "core/list/inc/LinkedList.h"

#define LinkedListItem2ShiftRegister(instance) \
    BASE_TO_SUB_CAST(instance, ShiftRegister, base)

typedef struct {
    LinkedListItem base;
    uint8_t _data;
} ShiftRegister;

// (De)constructors
PUBLIC void constructShiftRegister(ShiftRegister *instance);
PUBLIC void deconstructShiftRegister(ShiftRegister *instance);

// Public method(s)
PUBLIC void setBitToShiftRegister(ShiftRegister *pThis, unsigned int bit);
PUBLIC void resetBitFromShiftRegister(ShiftRegister *pThis, unsigned int bit);
PUBLIC uint8_t getDataFromShiftRegister(ShiftRegister *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SHIFT_REGISTER_H__

