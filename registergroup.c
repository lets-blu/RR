#include "registergroup.h"

#define __SLL(data) (data <<= 1)
#define __MSB(data) (data & 0x80)

// private method(s)
PRIVATE ShiftRegister * getRegisterFromGroup(RegisterGroup * pThis, uint8_t bit);
PRIVATE void prepareRegisterGroupSer(RegisterGroup * pThis, uint8_t data);
PRIVATE void generateRegisterGroupSrclk(RegisterGroup * pThis);
PRIVATE void generateRegisterGroupRclk(RegisterGroup * pThis);

PUBLIC RegisterGroup newRegisterGroup(GPIOPin pins[])
{
    RegisterGroup group = {
        ._head              = NULL,
        ._numOfRegisters    = 0, 

        ._ePin              = pins[REGGRP_E_PIN], 
        ._serPin            = pins[REGGRP_SER_PIN],
        ._srclkPin          = pins[REGGRP_SRCLK_PIN],
        ._rclkPin           = pins[REGGRP_RCLK_PIN]
    };

    setupGPIOPin(&group._ePin, OUTPUT);
    setupGPIOPin(&group._serPin, OUTPUT);
    setupGPIOPin(&group._srclkPin, OUTPUT);
    setupGPIOPin(&group._rclkPin, OUTPUT);

    return group;
}

PUBLIC void addRegisterToGroup(RegisterGroup * pThis, 
    ShiftRegister * shiftRegister)
{
    shiftRegister->next = pThis->_head;
    pThis->_head = shiftRegister;

    ++pThis->_numOfRegisters;
}

PUBLIC void setRegisterGroupBit(RegisterGroup * pThis, uint8_t bit)
{
    assert_param(bit < pThis->_numOfRegisters * 8);
    ShiftRegister * p = getRegisterFromGroup(pThis, bit);

    if (p != NULL) {
        setShiftRegisterBit(p, bit % 8);
    }
}

PUBLIC void resetRegisterGroupBit(RegisterGroup * pThis, uint8_t bit)
{
    assert_param(bit < pThis->_numOfRegisters * 8);
    ShiftRegister * p = getRegisterFromGroup(pThis, bit);

    if (p != NULL) {
        resetShiftRegisterBit(p, bit % 8);
    }
}

PUBLIC void setRegisterGroupOutput(RegisterGroup * pThis, 
    FunctionalState newState)
{
    writeGPIOPin(&pThis->_ePin, newState ? LOW : HIGH);
}

PUBLIC void outputRegisterGroup(RegisterGroup * pThis)
{
    for (ShiftRegister * p = pThis->_head; p != NULL; p = p->next) {
        uint8_t data = p->_data;

        for (uint8_t i = 0; i < 8; i++) {
            prepareRegisterGroupSer(pThis, data);
            generateRegisterGroupSrclk(pThis);

            __SLL(data);
        }
    }

    generateRegisterGroupRclk(pThis);
}

PRIVATE ShiftRegister * getRegisterFromGroup(RegisterGroup * pThis, uint8_t bit)
{
    ShiftRegister * p = pThis->_head;
    uint8_t i = pThis->_numOfRegisters - 1;

    for (; i > bit / 8 && p != NULL; i--, p = p->next) {
        ;// do nothing
    }

    return p;
}

PRIVATE void prepareRegisterGroupSer(RegisterGroup * pThis, uint8_t data)
{
    writeGPIOPin(&pThis->_serPin, __MSB(data) ? HIGH : LOW);
}

PRIVATE void generateRegisterGroupSrclk(RegisterGroup * pThis)
{
    writeGPIOPin(&pThis->_srclkPin, LOW);
    writeGPIOPin(&pThis->_srclkPin, HIGH);
}

PRIVATE void generateRegisterGroupRclk(RegisterGroup * pThis)
{
    writeGPIOPin(&pThis->_rclkPin, LOW);
    writeGPIOPin(&pThis->_rclkPin, HIGH);
}

PUBLIC STATIC void vTestRegisterGroupTask(void * pRegisterGroup)
{
    RegisterGroup * group = (RegisterGroup *)pRegisterGroup;

    if (group->_numOfRegisters == 0) {
        vTaskDelete(NULL);
    }

    for (;;) {
        for (uint8_t i = 0; i < 8; ++i) {

            // clear all bits
            for (uint8_t j = 0; j < 8; ++j) {
                resetRegisterGroupBit(group, j);
            }

            setRegisterGroupBit(group, i);
            outputRegisterGroup(group);
            vTaskDelay(1000);
        }
    }
}
