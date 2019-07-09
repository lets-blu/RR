#ifndef __SHIFTREGISTER_H__
#define __SHIFTREGISTER_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdint.h"
#include "stdlib.h"

#include "stm32f1xx_hal.h"

#include "keywords.h"

typedef struct ShiftRegister
{
    uint8_t _data;
    struct ShiftRegister * next;
} ShiftRegister;

// constructor
PUBLIC ShiftRegister newShiftRegister(void);

// public method(s)
PUBLIC void setShiftRegisterBit(ShiftRegister * pThis, uint8_t bit);
PUBLIC void resetShiftRegisterBit(ShiftRegister * pThis, uint8_t bit);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SHIFTREGISTER_H__
