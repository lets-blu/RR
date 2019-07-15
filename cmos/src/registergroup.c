#include "registergroup.h"

#define __REGGRP_SLL(data)  ((data) <<= 1)
#define __REGGRP_MSB(data)  ((data) & 0x80)

// private method(s)
PRIVATE ShiftRegister * getRegisterFromGroup(RegisterGroup * pThis, uint8_t bit);

PRIVATE void prepareRegisterGroupSer(RegisterGroup * pThis, uint8_t data);
PRIVATE void generateRegisterGroupSck(RegisterGroup * pThis);
PRIVATE void generateRegisterGroupRck(RegisterGroup * pThis);

PUBLIC RegisterGroup newRegisterGroup(GPIOPin pins[])
{
    RegisterGroup group = 
    {
        ._registers         = NULL, 
        ._registersCount    = 0, 

        ._oePin             = pins[REGGRP_OE_PIN], 
        ._serPin            = pins[REGGRP_SER_PIN], 
        ._sckPin            = pins[REGGRP_SCK_PIN], 
        ._rckPin            = pins[REGGRP_RCK_PIN]
    };

    setupGPIOPin(&pins[REGGRP_OE_PIN], OUTPUT);
    setupGPIOPin(&pins[REGGRP_SER_PIN], OUTPUT);
    setupGPIOPin(&pins[REGGRP_SCK_PIN], OUTPUT);
    setupGPIOPin(&pins[REGGRP_RCK_PIN], OUTPUT);

    return group;
}

PUBLIC void addRegisterToGroup(RegisterGroup * pThis, ShiftRegister * shiftRegister)
{
    shiftRegister->next = pThis->_registers;
    pThis->_registers = shiftRegister;

    pThis->_registersCount++;
}

PUBLIC void setRegisterGroupBit(RegisterGroup * pThis, uint8_t bit)
{
    assert_param(bit < pThis->_registersCount * 8);
    ShiftRegister * shiftRegister = getRegisterFromGroup(pThis, bit);

    if (shiftRegister != NULL)
    {
        setShiftRegisterBit(shiftRegister, bit % 8);
    }
}

PUBLIC void resetRegisterGroupBit(RegisterGroup * pThis, uint8_t bit)
{
    assert_param(bit < pThis->_registersCount * 8);
    ShiftRegister * shiftRegister = getRegisterFromGroup(pThis, bit);

    if (shiftRegister != NULL)
    {
        resetShiftRegisterBit(shiftRegister, bit % 8);
    }
}

PUBLIC void setRegisterGroupOutput(RegisterGroup * pThis, FunctionalState state)
{
    writeGPIOPin(&pThis->_oePin, (state != DISABLE) ? LOW : HIGH);
}

PUBLIC void outputRegisterGroup(RegisterGroup * pThis)
{
    for (ShiftRegister * reg = pThis->_registers; reg != NULL; reg = reg->next)
    {
        uint8_t data = reg->_data;

        for (uint8_t i = 0; i < 8; i++)
        {
            prepareRegisterGroupSer(pThis, data);
            generateRegisterGroupSck(pThis);

            __REGGRP_SLL(data);
        }
    }

    generateRegisterGroupRck(pThis);
}

PRIVATE ShiftRegister * getRegisterFromGroup(RegisterGroup * pThis, uint8_t bit)
{
    ShiftRegister * reg = pThis->_registers;
    uint8_t iStart = pThis->_registersCount - 1;

    for (uint8_t i = iStart; i > bit / 8 && reg != NULL; i--, reg = reg->next)
    {
        // do nothing here
    }

    return reg;
}

PRIVATE void prepareRegisterGroupSer(RegisterGroup * pThis, uint8_t data)
{
    writeGPIOPin(&pThis->_serPin, (__REGGRP_MSB(data) == 0x00) ? LOW : HIGH);
}

PRIVATE void generateRegisterGroupSck(RegisterGroup * pThis)
{
    writeGPIOPin(&pThis->_sckPin, LOW);
    writeGPIOPin(&pThis->_sckPin, HIGH);
}

PRIVATE void generateRegisterGroupRck(RegisterGroup * pThis)
{
    writeGPIOPin(&pThis->_rckPin, LOW);
    writeGPIOPin(&pThis->_rckPin, HIGH);
}
