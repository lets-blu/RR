#include "registergroup.h"

#define __SLL(data) (data <<= 1)
#define __SLR(data) (data >>= 1)
#define __MSB(data) (data & 0x80)
#define __LSB(data) (data & 0x01)

// private method(s)
PRIVATE ShiftRegister * getRegisterFromGroup(RegisterGroup * this, uint8_t bit);
PRIVATE void prepareRegisterGroupSer(RegisterGroup * this, uint8_t data);
PRIVATE void generateRegisterGroupSck(RegisterGroup * this);
PRIVATE void generateRegisterGroupRck(RegisterGroup * this);

PUBLIC RegisterGroup newRegisterGroup(GPIOPin pins[NUM_OF_REGGRP_PINS]) {
    RegisterGroup group = {
        ._head              = NULL, 
        ._numOfRegisters    = 0, 
        ._serPin            = pins[REGGRP_SER_PIN], 
        ._sckPin            = pins[REGGRP_SCK_PIN], 
        ._rckPin            = pins[REGGRP_RCK_PIN], 
        ._ePin              = pins[REGGRP_E_PIN]
    };

    configGPIOPin(&group._serPin, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
    configGPIOPin(&group._sckPin, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
    configGPIOPin(&group._rckPin, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
    configGPIOPin(&group._ePin, GPIO_Speed_2MHz, GPIO_Mode_Out_OD);

    return group;
}

PUBLIC void addRegisterToGroup(RegisterGroup * this, ShiftRegister * shiftRegister) {
    shiftRegister->next = this->_head;
    this->_head = shiftRegister;
    
    this->_numOfRegisters++;
}

PUBLIC void setRegisterGroupBit(RegisterGroup * this, uint8_t bit) {
    assert_param(bit < this->_numOfRegisters * 8);

    ShiftRegister * p = getRegisterFromGroup(this, bit);
    if (p != NULL) {
        setShiftRegisterBit(p, bit % 8);
    }
}

PUBLIC void resetRegisterGroupBit(RegisterGroup * this, uint8_t bit) {
    assert_param(bit < this->_numOfRegisters * 8);
    
    ShiftRegister * p = getRegisterFromGroup(this, bit);
    if (p != NULL) {
        resetShiftRegisterBit(p, bit % 8);
    }
}

PUBLIC void setRegisterGroupEnable(RegisterGroup * this, FunctionalState newState) {
    writeGPIOPin(&this->_ePin, newState ? LOW : HIGH);
}

PUBLIC void outputRegisterGroup(RegisterGroup * this) {
    for (ShiftRegister * p = this->_head; p != NULL; p = p->next) {
        uint8_t data = p->_data;

        for (uint8_t i = 0; i < 8; i++) {
            prepareRegisterGroupSer(this, data);
            generateRegisterGroupSck(this);
            __SLL(data);
        }
    }

    generateRegisterGroupRck(this);
}

PRIVATE ShiftRegister * getRegisterFromGroup(RegisterGroup * this, uint8_t bit) {
    ShiftRegister * p = this->_head;
    for (uint8_t i = this->_numOfRegisters - 1; i > bit / 8 && p != NULL; i--, p = p->next);

    return p;
}

PRIVATE void prepareRegisterGroupSer(RegisterGroup * this, uint8_t data) {
    writeGPIOPin(&this->_serPin, __MSB(data) ? HIGH : LOW);
}

PRIVATE void generateRegisterGroupSck(RegisterGroup * this) {
    writeGPIOPin(&this->_sckPin, LOW);
    writeGPIOPin(&this->_sckPin, HIGH);
}

PRIVATE void generateRegisterGroupRck(RegisterGroup * this) {
    writeGPIOPin(&this->_rckPin, LOW);
    writeGPIOPin(&this->_rckPin, HIGH);
}
