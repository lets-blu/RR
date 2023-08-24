#include "core/device/inc/BaseScannable.h"

// Method implement(s)
PROTECTED void constructBaseScannable(BaseScannable *instance)
{
    if (instance != NULL) {
        constructLinkedListItem(&instance->base);
        instance->vtbl = NULL;
    }
}

PROTECTED void deconstructBaseScannable(BaseScannable *instance)
{
    if (instance != NULL) {
        deconstructLinkedListItem(&instance->base);
        memset(instance, 0, sizeof(BaseScannable));
    }
}

