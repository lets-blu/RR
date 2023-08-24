#ifndef __DATA_SELECTOR_H__
#define __DATA_SELECTOR_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "core/list/inc/LinkedList.h"
#include "core/log/inc/LogFilter.h"

#include "digital/combinatorial/inc/DataSelectorHandler.h"

#define LinkedListItem2DataSelector(instance) \
    BASE_TO_SUB_CAST(instance, DataSelector, base)

typedef struct {
    LinkedListItem base;

    unsigned int _data;
    unsigned int _startAddress;
    unsigned int _endAddress;

    LinkedList _changeHandlers;
} DataSelector;

// (De)constructors
PUBLIC void constructDataSelector(
    DataSelector *instance,
    unsigned int startAddress,
    unsigned int endAddress);

PUBLIC void deconstructDataSelector(DataSelector *instance);

// Public method(s)
PUBLIC void addChangeHandlerToDataSelector(
    DataSelector *pThis, DataSelectorHandler *handler);

PUBLIC void removeChangeHandlerFromDataSelector(
    DataSelector *pThis, DataSelectorHandler *handler);

PUBLIC unsigned int getStartAddressFromDataSelector(
    DataSelector *pThis);

PUBLIC unsigned int getEndAddressFromDataSelector(
    DataSelector *pThis);

PUBLIC void notifyChangeToDataSelector(
    DataSelector *pThis, unsigned int address, unsigned int data);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DATA_SELECTOR_H__

