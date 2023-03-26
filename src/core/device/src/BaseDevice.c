#include "core/device/inc/BaseDevice.h"

// Protected method(s)
PROTECTED void constructBaseDevice(BaseDevice *instance);
PROTECTED void deconstructBaseDevice(BaseDevice *instance);

// Method implement(s)
PROTECTED void constructBaseDevice(BaseDevice *instance)
{
    if (instance != NULL) {
        constructLinkedListItem(&instance->base);
        instance->_referenceCount = 0;
    }
}

PROTECTED void deconstructBaseDevice(BaseDevice *instance)
{
    if (instance != NULL) {
        deconstructLinkedListItem(&instance->base);
        memset(instance, 0, sizeof(BaseDevice));
    }
}

