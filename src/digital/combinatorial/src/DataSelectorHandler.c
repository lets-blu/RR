#include "digital/combinatorial/inc/DataSelectorHandler.h"

// Method implement(s)
PUBLIC void constructDataSelectorHandler(
    DataSelectorHandler *instance,
    unsigned int address,
    EventHandlerCallback callback)
{
    if (instance != NULL) {
        constructEventHandler(&instance->base, callback);
        instance->_address = address;
    }
}

PUBLIC void deconstructDataSelectorHandler(
    DataSelectorHandler *instance)
{
    if (instance != NULL) {
        deconstructEventHandler(&instance->base);
        memset(instance, 0, sizeof(DataSelectorHandler));
    }
}

PUBLIC unsigned int getAddressFromDataSelectorHandler(
    DataSelectorHandler *pThis)
{
    return (pThis == NULL) ? 0 : pThis->_address;
}

