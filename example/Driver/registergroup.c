#include "registergroup.h"

#define NUM_OF_PINS ((uint8_t)4)
#define SER_PIN     ((uint8_t)0)
#define SCK_PIN     ((uint8_t)1)
#define RCK_PIN     ((uint8_t)2)
#define E_PIN       ((uint8_t)3)

#define __SLL(data) (data <<= 1)
#define __SLR(data) (data >>= 1)
#define __MSB(data) (data & 0x80)
#define __LSB(data) (data & 0x01)

// (de)constructor(s)
PRIVATE RegisterGroup newRegisterGroup(GPIOPin pins[]);

// private method(s)
PRIVATE void addRegisterToGroup(RegisterGroup * this, ShiftRegister * shiftRegister);
PRIVATE void setRegisterGroupEnable(RegisterGroup * this, FunctionalState newState);
PRIVATE void prepareRegisterGroupSer(RegisterGroup * this, uint8_t data);
PRIVATE void generateRegisterGroupSck(RegisterGroup * this);
PRIVATE void generateRegisterGroupRck(RegisterGroup * this);

// static member(s)
PUBLIC STATIC RegisterGroup registerGroup1;

PUBLIC void staticRegisterGroup(void) {
    static bool called = false;

    if (called) {
        return;
    }

    GPIOPin pins[NUM_OF_PINS] = {
        [SER_PIN]   = newGPIOPin(GPIOA, GPIO_Pin_7), 
        [SCK_PIN]   = newGPIOPin(GPIOA, GPIO_Pin_5), 
        [RCK_PIN]   = newGPIOPin(GPIOA, GPIO_Pin_6), 
        [E_PIN]     = newGPIOPin(GPIOA, GPIO_Pin_4) 
    };

    registerGroup1 = newRegisterGroup(pins);

    staticShiftRegister();
    addRegisterToGroup(&registerGroup1, &shiftRegister1);
    addRegisterToGroup(&registerGroup1, &shiftRegister2);
    addRegisterToGroup(&registerGroup1, &shiftRegister3);
    addRegisterToGroup(&registerGroup1, &shiftRegister4);

    setRegisterGroupEnable(&registerGroup1, ENABLE);

    called = true;
}

PRIVATE RegisterGroup newRegisterGroup(GPIOPin pins[NUM_OF_PINS]) {
    RegisterGroup group = {
        ._head              = NULL, 
        ._numOfRegisters    = 0, 
        ._serPin            = pins[SER_PIN], 
        ._sckPin            = pins[SCK_PIN], 
        ._rckPin            = pins[RCK_PIN], 
        ._ePin              = pins[E_PIN]
    };

    configGPIOPin(&group._serPin, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
    configGPIOPin(&group._sckPin, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
    configGPIOPin(&group._rckPin, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
    configGPIOPin(&group._ePin, GPIO_Speed_2MHz, GPIO_Mode_Out_OD);

    return group;
}

PUBLIC void setRegisterGroupBit(RegisterGroup * this, uint8_t bit) {
    assert_param(bit < this->_numOfRegisters * 8);
    ShiftRegister * p = this->_head;

    for (uint8_t i = this->_numOfRegisters - 1; i > bit / 8 && p != NULL; i--, p = p->next);
    if (p != NULL) {
        setShiftRegisterBit(p, bit % 8);
    }
}

PUBLIC void resetRegisterGroupBit(RegisterGroup * this, uint8_t bit) {
    assert_param(bit < this->_numOfRegisters * 8);
    ShiftRegister * p = this->_head;

    for (uint8_t i = this->_numOfRegisters - 1; i > bit / 8 && p != NULL; i--, p = p->next);
    if (p != NULL) {
        resetShiftRegisterBit(p, bit % 8);
    }
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

PRIVATE void addRegisterToGroup(RegisterGroup * this, ShiftRegister * shiftRegister) {
    if (this->_head == NULL) {
        this->_head = shiftRegister;
    } else {
        shiftRegister->next = this->_head;
        this->_head = shiftRegister;
    }
    
    this->_numOfRegisters++;
}

PRIVATE void setRegisterGroupEnable(RegisterGroup * this, FunctionalState newState) {
    if (newState == ENABLE) {
        GPIO_ResetBits(this->_ePin._port, this->_ePin._pin);
    } else {
        GPIO_SetBits(this->_ePin._port, this->_ePin._pin);
    }
}

PRIVATE void prepareRegisterGroupSer(RegisterGroup * this, uint8_t data) {
    if (__MSB(data)) {
        GPIO_SetBits(this->_serPin._port, this->_serPin._pin);
    } else {
        GPIO_ResetBits(this->_serPin._port, this->_serPin._pin);
    }
}

PRIVATE void generateRegisterGroupSck(RegisterGroup * this) {
    GPIO_ResetBits(this->_sckPin._port, this->_sckPin._pin);
    GPIO_SetBits(this->_sckPin._port, this->_sckPin._pin);

}

PRIVATE void generateRegisterGroupRck(RegisterGroup * this) {
    GPIO_ResetBits(this->_rckPin._port, this->_rckPin._pin);
    GPIO_SetBits(this->_rckPin._port, this->_rckPin._pin);
}
