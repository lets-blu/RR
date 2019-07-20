#include "registergroup.h"

#define __REGGRP_SLL(data)  ((data) <<= 1)
#define __REGGRP_MSB(data)  ((data) & 0x80)

// private method(s)
PRIVATE ShiftRegister * getRegisterFromGroup(RegisterGroup * pThis, uint8_t bit);

PRIVATE void prepareRegisterGroupSer(RegisterGroup * pThis, uint8_t data);
PRIVATE void generateRegisterGroupSck(RegisterGroup * pThis);
PRIVATE void generateRegisterGroupRck(RegisterGroup * pThis);

PUBLIC RegisterGroup newRegisterGroup(RegisterGroupPins pins)
{
    RegisterGroup group = 
    {
        ._registers         = NULL, 
        ._registersCount    = 0, 

        ._oePin             = pins.oePin, 
        ._serPin            = pins.serPin, 
        ._sckPin            = pins.sckPin, 
        ._rckPin            = pins.rckPin
    };

    setupGPIOPin(&pins.oePin, OUTPUT);
    setupGPIOPin(&pins.serPin, OUTPUT);
    setupGPIOPin(&pins.sckPin, OUTPUT);
    setupGPIOPin(&pins.rckPin, OUTPUT);

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

        for (uint8_t bit = 0; bit < 8; bit++)
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
    uint8_t count = pThis->_registersCount - 1;

    for (uint8_t i = count; i > bit / 8 && reg != NULL; i--, reg = reg->next)
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
