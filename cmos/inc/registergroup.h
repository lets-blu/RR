#ifndef __REGISTERGROUP_H__
#define __REGISTERGROUP_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdint.h"
#include "stdlib.h"

#include "stm32f1xx_hal.h"

#include "gpiopin.h"
#include "keywords.h"
#include "shiftregister.h"

typedef struct 
{
    ShiftRegister * _registers;
    uint8_t _registersCount;

    GPIOPin _oePin;
    GPIOPin _serPin, _sckPin, _rckPin;
} RegisterGroup;

typedef struct
{
    GPIOPin oePin;
    GPIOPin serPin, sckPin, rckPin;
} RegisterGroupPins;

// constructor
PUBLIC RegisterGroup newRegisterGroup(RegisterGroupPins pins);

// public method(s)
PUBLIC void addRegisterToGroup(RegisterGroup * pThis, ShiftRegister * shiftRegister);

PUBLIC void setRegisterGroupBit(RegisterGroup * pThis, uint8_t bit);
PUBLIC void resetRegisterGroupBit(RegisterGroup * pThis, uint8_t bit);

PUBLIC void setRegisterGroupOutput(RegisterGroup * pThis, FunctionalState state);
PUBLIC void outputRegisterGroup(RegisterGroup * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __REGISTERGROUP_H__
