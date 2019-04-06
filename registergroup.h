#ifndef __REGISTERGROUP_H__
#define __REGISTERGROUP_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "assert.h"
#include "stdlib.h"
#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "task.h"

#include "gpiopin.h"
#include "keywords.h"
#include "shiftregister.h"

#define NUM_OF_REGGRP_PINS ((uint8_t)4)
#define REGGRP_SER_PIN ((uint8_t)0)
#define REGGRP_SRCLK_PIN ((uint8_t)1)
#define REGGRP_RCLK_PIN ((uint8_t)2)
#define REGGRP_E_PIN ((uint8_t)3)

    typedef struct RegisterGroup
    {
        ShiftRegister *_head;
        uint8_t _numOfRegisters;

        GPIOPin _ePin;
        GPIOPin _serPin, _srclkPin, _rclkPin;
    } RegisterGroup;

    // (de)constructor(s)
    PUBLIC RegisterGroup newRegisterGroup(GPIOPin pins[NUM_OF_REGGRP_PINS]);

    // public method(s)
    PUBLIC void addRegisterToGroup(RegisterGroup *pThis,
                                   ShiftRegister *shiftRegister);
    PUBLIC void setRegisterGroupBit(RegisterGroup *pThis, uint8_t bit);
    PUBLIC void resetRegisterGroupBit(RegisterGroup *pThis, uint8_t bit);
    PUBLIC void setRegisterGroupOutput(RegisterGroup *pThis,
                                       FunctionalState newState);
    PUBLIC void outputRegisterGroup(RegisterGroup *pThis);

    PUBLIC STATIC void vTestRegisterGroupTask(void *pRegisterGroup);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __REGISTERGROUP_H__
