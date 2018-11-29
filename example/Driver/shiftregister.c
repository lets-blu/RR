#include "shiftregister.h"

// (de)constructor(s)
PRIVATE ShiftRegister newShiftRegister(void);

// static member(s)
PUBLIC STATIC ShiftRegister shiftRegister1, shiftRegister2, shiftRegister3, 
        shiftRegister4;

PUBLIC void staticShiftRegister(void) {
    static bool called = false;

    if (called) {
        return;
    }

    shiftRegister1 = newShiftRegister();
    shiftRegister2 = newShiftRegister();
    shiftRegister3 = newShiftRegister();
    shiftRegister4 = newShiftRegister();

    called = true;
}

PRIVATE ShiftRegister newShiftRegister(void) {
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
