#include "selectormessage.h"

PUBLIC SelectorMessage newSelectorMessage(uint8_t id, GPIOPinState data) {
    SelectorMessage message = {
        ._id = id,
        ._data = data
    };

    return message;
}
