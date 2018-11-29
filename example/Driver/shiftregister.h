#ifndef __SHIFTREGISTER_H__
#define __SHIFTREGISTER_H__

#include "assert.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stm32f10x.h"

#include "keyword.h"

typedef struct ShiftRegister {
    uint8_t _data;
    struct ShiftRegister * next;
} ShiftRegister;

extern PUBLIC STATIC ShiftRegister shiftRegister1, shiftRegister2, 
        shiftRegister3, shiftRegister4;
PUBLIC void staticShiftRegister(void);

PUBLIC void setShiftRegisterBit(ShiftRegister * this, uint8_t bit);
PUBLIC void resetShiftRegisterBit(ShiftRegister * this, uint8_t bit);

#endif // __SHIFTREGISTER_H__
