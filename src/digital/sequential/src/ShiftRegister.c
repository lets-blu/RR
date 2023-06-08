#include "digital/sequential/inc/ShiftRegister.h"

// Method implement(s)
PUBLIC void constructShiftRegister(ShiftRegister *instance)
{
    if (instance != NULL) {
        constructLinkedListItem(&instance->base);
        instance->_data = 0;
    }
}

PUBLIC void deconstructShiftRegister(ShiftRegister *instance)
{
    if (instance != NULL) {
        deconstructLinkedListItem(&instance->base);
        memset(instance, 0, sizeof(ShiftRegister));
    }
}

PUBLIC void setBitToShiftRegister(ShiftRegister *pThis, unsigned int bit)
{
    if (pThis != NULL && bit < 8) {
        pThis->_data |= (1 << bit);
    }
}

PUBLIC void resetBitFromShiftRegister(ShiftRegister *pThis, unsigned int bit)
{
    if (pThis != NULL && bit < 8) {
        pThis->_data &= ~(1 << bit);
    }
}

PUBLIC uint8_t getDataFromShiftRegister(ShiftRegister *pThis)
{
    return (pThis == NULL) ? 0 : pThis->_data;
}

