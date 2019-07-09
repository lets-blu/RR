#include "shiftregister.h"

PUBLIC ShiftRegister newShiftRegister(void)
{
    ShiftRegister reg = 
    {
        ._data  = 0x00, 
        .next   = NULL
    };

    return reg;
}

PUBLIC void setShiftRegisterBit(ShiftRegister * pThis, uint8_t bit)
{
    assert_param(bit < 8);
    pThis->_data |= (1 << bit);
}

PUBLIC void resetShiftRegisterBit(ShiftRegister * pThis, uint8_t bit)
{
    assert_param(bit < 8);
    pThis->_data &= ~(1 << bit);
}
