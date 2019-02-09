#include "registergroup.h"

#define __SLL(data) (data <<= 1)
#define __SLR(data) (data >>= 1)
#define __MSB(data) (data & 0x80)
#define __LSB(data) (data & 0x01)

// private method(s)
PRIVATE ShiftRegister * getRegisterFromGroup(RegisterGroup * pThis, uint8_t bit);
PRIVATE void prepareRegisterGroupSer(RegisterGroup * pThis, uint8_t data);
PRIVATE void generateRegisterGroupSck(RegisterGroup * pThis);
PRIVATE void generateRegisterGroupRck(RegisterGroup * pThis);

PUBLIC RegisterGroup newRegisterGroup(GPIOPin pins[NUM_OF_REGGRP_PINS]) {
    RegisterGroup group = {
        ._head = NULL,
        ._numOfRegisters = 0,
        ._serPin = pins[REGGRP_SER_PIN],
        ._sckPin = pins[REGGRP_SCK_PIN],
        ._rckPin = pins[REGGRP_RCK_PIN],
        ._ePin = pins[REGGRP_E_PIN]
    };

    configGPIOPin(&group._serPin, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
    configGPIOPin(&group._sckPin, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
    configGPIOPin(&group._rckPin, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
    configGPIOPin(&group._ePin, GPIO_Speed_2MHz, GPIO_Mode_Out_OD);

    return group;
}

PUBLIC void addRegisterToGroup(RegisterGroup * pThis, ShiftRegister * shiftRegister) {
    shiftRegister->next = pThis->_head;
    pThis->_head = shiftRegister;

    pThis->_numOfRegisters++;
}

PUBLIC void setRegisterGroupBit(RegisterGroup * pThis, uint8_t bit) {
    assert_param(bit < pThis->_numOfRegisters * 8);

    ShiftRegister * p = getRegisterFromGroup(pThis, bit);
    if (p != NULL) {
        setShiftRegisterBit(p, bit % 8);
    }
}

PUBLIC void resetRegisterGroupBit(RegisterGroup * pThis, uint8_t bit) {
    assert_param(bit < pThis->_numOfRegisters * 8);

    ShiftRegister * p = getRegisterFromGroup(pThis, bit);
    if (p != NULL) {
        resetShiftRegisterBit(p, bit % 8);
    }
}

PUBLIC void setRegisterGroupEnable(RegisterGroup * pThis, FunctionalState newState) {
    writeGPIOPin(&pThis->_ePin, newState ? LOW : HIGH);
}

PUBLIC void outputRegisterGroup(RegisterGroup * pThis) {
    for (ShiftRegister * p = pThis->_head; p != NULL; p = p->next) {
        uint8_t data = p->_data;

        for (uint8_t i = 0; i < 8; i++) {
            prepareRegisterGroupSer(pThis, data);
            generateRegisterGroupSck(pThis);
            __SLL(data);
        }
    }

    generateRegisterGroupRck(pThis);
}

PRIVATE ShiftRegister * getRegisterFromGroup(RegisterGroup * pThis, uint8_t bit) {
    ShiftRegister * p = pThis->_head;
    for (uint8_t i = pThis->_numOfRegisters - 1; i > bit / 8 && p != NULL; i--, p = p->next);

    return p;
}

PRIVATE void prepareRegisterGroupSer(RegisterGroup * pThis, uint8_t data) {
    writeGPIOPin(&pThis->_serPin, __MSB(data) ? HIGH : LOW);
}

PRIVATE void generateRegisterGroupSck(RegisterGroup * pThis) {
    writeGPIOPin(&pThis->_sckPin, LOW);
    writeGPIOPin(&pThis->_sckPin, HIGH);
}

PRIVATE void generateRegisterGroupRck(RegisterGroup * pThis) {
    writeGPIOPin(&pThis->_rckPin, LOW);
    writeGPIOPin(&pThis->_rckPin, HIGH);
}
