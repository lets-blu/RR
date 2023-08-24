#include "digital/combinatorial/inc/DataSelector.h"

// Macro(s)
#define IS_VALID_ADDRESS(pThis, address) \
    ((pThis)->_startAddress <= (address) && (address) <= (pThis)->_endAddress)

// Private member(s)
PRIVATE STATIC LogFilter filter
    = STATIC_LOG_FILTER("DataSelector", LOG_LEVEL_INFO);

// Method implement(s)
PUBLIC void constructDataSelector(
    DataSelector *instance,
    unsigned int startAddress,
    unsigned int endAddress)
{
    if (instance == NULL || startAddress > endAddress) {
        return;
    }

    // 1. construct base
    constructLinkedListItem(&instance->base);

    // 2. construct member(s)
    instance->_data = 0;
    instance->_startAddress = startAddress;
    instance->_endAddress = endAddress;

    constructLinkedList(&instance->_changeHandlers);
}

PUBLIC void deconstructDataSelector(DataSelector *instance)
{
    if (instance != NULL) {
        deconstructLinkedListItem(&instance->base);
        deconstructLinkedList(&instance->_changeHandlers);
        memset(instance, 0, sizeof(DataSelector));
    }
}

PUBLIC void addChangeHandlerToDataSelector(
    DataSelector *pThis, DataSelectorHandler *handler)
{
    unsigned int address = 0;

    if (pThis == NULL || handler == NULL) {
        return;
    }

    address = getAddressFromDataSelectorHandler(handler);

    if (IS_VALID_ADDRESS(pThis, address)) {
        addItemToLinkedList(&pThis->_changeHandlers, &handler->base.base);
    }
}

PUBLIC void removeChangeHandlerFromDataSelector(
    DataSelector *pThis, DataSelectorHandler *handler)
{
    if (pThis != NULL && handler != NULL) {
        removeItemFromLinkedList(&pThis->_changeHandlers, &handler->base.base);
    }
}

PUBLIC unsigned int getStartAddressFromDataSelector(DataSelector *pThis)
{
    return (pThis == NULL) ? 0 : pThis->_startAddress;
}

PUBLIC unsigned int getEndAddressFromDataSelector(DataSelector *pThis)
{
    return (pThis == NULL) ? 0 : pThis->_endAddress;
}

PUBLIC void notifyChangeToDataSelector(
    DataSelector *pThis, unsigned int address, unsigned int data)
{
    unsigned int mask = 0;

    if (pThis == NULL || !IS_VALID_ADDRESS(pThis, address)) {
        return;
    }

    mask = 1 << (address - pThis->_startAddress);
    data = (data & 1) << (address - pThis->_startAddress);

    if (((data ^ pThis->_data) & mask) != 0) {
        LinkedListIterator iterator;
        constructLinkedListIterator(&iterator, &pThis->_changeHandlers);

        while (hasNextOfLinkedListIterator(&iterator)) {
            DataSelectorHandler *handler = LinkedListItem2DataSelectorHandler(
                nextOfLinkedListIterator(&iterator));
            
            if (address == getAddressFromDataSelectorHandler(handler)) {
                handler->base.callback(&handler->base, pThis, NULL);
            }
        }

        deconstructLinkedListIterator(&iterator);
    }

    if (data != 0) {
        pThis->_data |= mask;
    } else {
        pThis->_data &= ~mask;
    }

    if (address == pThis->_endAddress) {
        LOG_I(&filter, "notify change, data of selector 0x%x is 0x%x",
            pThis, pThis->_data);
    }
}

