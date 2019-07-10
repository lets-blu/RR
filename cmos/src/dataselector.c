#include "dataselector.h"

PUBLIC DataSelector newDataSelector(uint8_t startAddress, uint8_t endAddress)
{
    DataSelector selector = 
    {
        ._startAddress  = startAddress, 
        ._endAddress    = endAddress, 

        .next           = NULL
    };

    return selector;
}
