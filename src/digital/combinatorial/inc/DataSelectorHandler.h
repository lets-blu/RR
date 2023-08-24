#ifndef __DATA_SELECTOR_HANDLER_H__
#define __DATA_SELECTOR_HANDLER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "core/event/inc/EventHandler.h"

#define EventHandler2DataSelectorHandler(instance)      \
    BASE_TO_SUB_CAST(instance, DataSelectorHandler, base)

#define LinkedListItem2DataSelectorHandler(instance)    \
    EventHandler2DataSelectorHandler(LinkedListItem2EventHandler(instance))

typedef struct {
    EventHandler base;
    unsigned int _address;
} DataSelectorHandler;

// (De)constructors
PUBLIC void constructDataSelectorHandler(
    DataSelectorHandler *instance,
    unsigned int address,
    EventHandlerCallback callback);

PUBLIC void deconstructDataSelectorHandler(
    DataSelectorHandler *instance);

// Public method(s)
PUBLIC unsigned int getAddressFromDataSelectorHandler(
    DataSelectorHandler *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DATA_SELECTOR_HANDLER_H__

