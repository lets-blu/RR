#ifndef __REGISTERGROUP_H__
#define __REGISTERGROUP_H__

#ifdef __cplusplus
extern "C"
{
#endif /// __cplusplus

#include "stdint.h"
#include "stdlib.h"

#include "stm32f1xx_hal.h"

#include "gpiopin.h"
#include "keywords.h"
#include "shiftregister.h"

#define REGGRP_PINS_COUNT   ((uint8_t)4)
#define REGGRP_OE_PIN       ((uint8_t)0)
#define REGGRP_SER_PIN      ((uint8_t)1)
#define REGGRP_SCK_PIN      ((uint8_t)2)
#define REGGRP_RCK_PIN      ((uint8_t)3)

typedef struct 
{
    ShiftRegister * _registers;
    uint8_t _registersCount;

    GPIOPin _oePin;
    GPIOPin _serPin, _sckPin, _rckPin;
} RegisterGroup;

// constructor
PUBLIC RegisterGroup newRegisterGroup(GPIOPin pins[REGGRP_PINS_COUNT]);

// public method(s)
PUBLIC void addRegisterToGroup(RegisterGroup * pThis, ShiftRegister * reg);

PUBLIC void setRegisterGroupBit(RegisterGroup * pThis, uint8_t bit);
PUBLIC void resetRegisterGroupBit(RegisterGroup * pThis, uint8_t bit);

PUBLIC void setRegisterGroupOutput(RegisterGroup * pThis, FunctionalState state);
PUBLIC void outputRegisterGroup(RegisterGroup * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __REGISTERGROUP_H__
