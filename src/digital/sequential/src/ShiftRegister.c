#include "digital/sequential/inc/ShiftRegister.h"

// Method implement(s)
PUBLIC void constructShiftRegister(
    ShiftRegister *instance, unsigned int dataSize)
{
    if (instance != NULL && dataSize <= 8 * sizeof(unsigned int)) {
        constructLinkedListItem(&instance->base);
        instance->_data = 0;
        instance->_dataSize = dataSize;
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
    if (pThis != NULL && bit < pThis->_dataSize) {
        pThis->_data |= (1 << bit);
    }
}

PUBLIC void resetBitFromShiftRegister(ShiftRegister *pThis, unsigned int bit)
{
    if (pThis != NULL && bit < pThis->_dataSize) {
        pThis->_data &= ~(1 << bit);
    }
}

PUBLIC unsigned int getDataFromShiftRegister(ShiftRegister *pThis)
{
    return (pThis == NULL) ? 0 : pThis->_data;
}

PUBLIC unsigned int getDataSizeFromShiftRegister(ShiftRegister *pThis)
{
    return (pThis == NULL) ? 0 : pThis->_dataSize;
}

