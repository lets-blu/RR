#ifndef __REGISTERGROUP_H__
#define __REGISTERGROUP_H__

#include "assert.h"
#include "stdlib.h"
#include "stm32f10x.h"

#include "gpiopin.h"
#include "keywords.h"
#include "shiftregister.h"

#define NUM_OF_REGGRP_PINS ((uint8_t)4)
#define REGGRP_SER_PIN     ((uint8_t)0)
#define REGGRP_SCK_PIN     ((uint8_t)1)
#define REGGRP_RCK_PIN     ((uint8_t)2)
#define REGGRP_E_PIN       ((uint8_t)3)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct RegisterGroup {
    ShiftRegister * _head;
    uint8_t _numOfRegisters;
	
    GPIOPin _serPin, _sckPin, _rckPin, _ePin;
} RegisterGroup;

// (de)constructor(s)
PUBLIC RegisterGroup newRegisterGroup(GPIOPin pins[]);

// public method(s)
PUBLIC void addRegisterToGroup(RegisterGroup * this, ShiftRegister * shiftRegister);
PUBLIC void setRegisterGroupBit(RegisterGroup * this, uint8_t bit);
PUBLIC void resetRegisterGroupBit(RegisterGroup * this, uint8_t bit);
PUBLIC void setRegisterGroupEnable(RegisterGroup * this, FunctionalState newState);
PUBLIC void outputRegisterGroup(RegisterGroup * this);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __REGISTERGROUP_H__
