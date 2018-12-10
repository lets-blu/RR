#include "shiftregister.h"

PUBLIC ShiftRegister newShiftRegister(void) {
    ShiftRegister shiftRegister = {
        ._data  = 0x00, 
        .next   = NULL
    };

    return shiftRegister;
}

PUBLIC void setShiftRegisterBit(ShiftRegister * this, uint8_t bit) {
    assert_param(bit < 8);
    this->_data |= (1 << bit);
}

PUBLIC void resetShiftRegisterBit(ShiftRegister * this, uint8_t bit) {
    assert_param(bit < 8);
    this->_data &= ~(1 << bit);
}
