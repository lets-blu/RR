#include "selectormessage.h"

PUBLIC SelectorMessage newSelectorMessage(uint8_t id, GPIOPinState data) {
    SelectorMessage message = {
        .id     = id, 
        .data   = data
    };

    return message;
}
