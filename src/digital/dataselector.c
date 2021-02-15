#include "dataselector.h"

PUBLIC DataSelector newDataSelector(uint8_t startAddress, uint8_t endAddress)
{
    assert(startAddress <= endAddress);
    
    DataSelector sel = {
        ._startAddress  = startAddress,
        ._endAddress    = endAddress,
        ._next          = NULL
    };

    return sel;
}
