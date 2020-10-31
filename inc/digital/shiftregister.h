#ifndef __SHIFTREGISTER_H__
#define __SHIFTREGISTER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stddef.h"
#include "stdint.h"
#include "keywords.h"

typedef struct ShiftRegister {
    uint8_t _data;
    struct ShiftRegister * _next;
} ShiftRegister;

// Constructor
PUBLIC ShiftRegister newShiftRegister(void);

// Public method(s)
PUBLIC void setShiftRegisterBit(ShiftRegister * pThis, uint8_t bit);
PUBLIC void resetShiftRegisterBit(ShiftRegister * pThis, uint8_t bit);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SHIFTREGISTER_H__
