#ifndef __SHIFTREGISTER_H__
#define __SHIFTREGISTER_H__

#include "assert.h"
#include "stdlib.h"
#include "stm32f10x.h"

#include "keywords.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    typedef struct ShiftRegister {
        uint8_t _data;
        struct ShiftRegister * next;
    } ShiftRegister;

    // (de)constructor(s)
    PUBLIC ShiftRegister newShiftRegister(void);

    // public method(s)
    PUBLIC void setShiftRegisterBit(ShiftRegister * pThis, uint8_t bit);
    PUBLIC void resetShiftRegisterBit(ShiftRegister * pThis, uint8_t bit);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SHIFTREGISTER_H__
