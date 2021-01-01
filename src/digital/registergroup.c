#include "registergroup.h"

#define REGGRP_SLL(data)    ((data) <<= 1)
#define REGGRP_MSB(data)    ((data) & 0x80)

// Private method(s)
PRIVATE void prepareRegisterGroupSer(RegisterGroup * pThis, uint8_t data);
PRIVATE void generateRegisterGroupSck(RegisterGroup * pThis);
PRIVATE void generateRegisterGroupRck(RegisterGroup * pThis);

PRIVATE ShiftRegister * getRegisterGroupRegister(RegisterGroup * pThis, uint8_t bit);

PUBLIC RegisterGroup newRegisterGroup(RegisterGroupPins pins)
{
    RegisterGroup group = {
        ._pins              = pins,
        ._registersCount    = 0,
        ._registers         = NULL
    };

    setupGPIOPin(&pins.oePin, OUTPUT);
    setupGPIOPin(&pins.serPin, OUTPUT);
    setupGPIOPin(&pins.sckPin, OUTPUT);
    setupGPIOPin(&pins.rckPin, OUTPUT);
    setRegisterGroupOutputEnabled(&group, false);

    return group;
}

PUBLIC void addRegisterGroupRegister(RegisterGroup * pThis, ShiftRegister * reg)
{
    pThis->_registersCount++;
    reg->_next = pThis->_registers;
    pThis->_registers = reg;
}

PUBLIC void setRegisterGroupBit(RegisterGroup * pThis, uint8_t bit)
{
    ShiftRegister * reg = getRegisterGroupRegister(pThis, bit);

    if (reg != NULL)
    {
        setShiftRegisterBit(reg, bit % 8);
    }
}

PUBLIC void resetRegisterGroupBit(RegisterGroup * pThis, uint8_t bit)
{
    ShiftRegister * reg = getRegisterGroupRegister(pThis, bit);

    if (reg != NULL)
    {
        resetShiftRegisterBit(reg, bit % 8);
    }
}

PUBLIC void setRegisterGroupOutputEnabled(RegisterGroup * pThis, bool enabled)
{
    writeGPIOPin(&pThis->_pins.oePin, enabled ? LOW : HIGH);
}

PUBLIC void outputRegisterGroup(RegisterGroup * pThis)
{
    for (ShiftRegister * reg = pThis->_registers; reg != NULL; reg = reg->_next)
    {
        uint8_t data = reg->_data;

        for (uint8_t bit = 0; bit < 8; bit++)
        {
            prepareRegisterGroupSer(pThis, data);
            generateRegisterGroupSck(pThis);
            REGGRP_SLL(data);
        }
    }

    generateRegisterGroupRck(pThis);
}

PRIVATE void prepareRegisterGroupSer(RegisterGroup * pThis, uint8_t data)
{
    writeGPIOPin(&pThis->_pins.serPin, (REGGRP_MSB(data) == 0x00) ? LOW : HIGH);
}

PRIVATE void generateRegisterGroupSck(RegisterGroup * pThis)
{
    writeGPIOPin(&pThis->_pins.sckPin, LOW);
    writeGPIOPin(&pThis->_pins.sckPin, HIGH);
}

PRIVATE void generateRegisterGroupRck(RegisterGroup * pThis)
{
    writeGPIOPin(&pThis->_pins.rckPin, LOW);
    writeGPIOPin(&pThis->_pins.rckPin, HIGH);
}

PRIVATE ShiftRegister * getRegisterGroupRegister(RegisterGroup * pThis, uint8_t bit)
{
    ShiftRegister * reg = pThis->_registers;
    uint8_t count = pThis->_registersCount - 1;

    for (uint8_t i = count; i > bit / 8 && reg != NULL; i--, reg = reg->_next)
    {
        // do nothing here
    }

    return reg;
}
