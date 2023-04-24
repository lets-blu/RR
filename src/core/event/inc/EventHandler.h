#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "core/list/inc/LinkedList.h"

#define LinkedListItem2EventHandler(instance) \
    ((EventHandler *)((LinkedListItem *)(instance)))

typedef struct EventHandler {
    LinkedListItem base;
    void (*callback)(struct EventHandler *pThis, void *sender, void *argument);
} EventHandler;

typedef void (*EventHandlerCallback)(EventHandler *, void *, void *);

// (De)constructors
PUBLIC void constructEventHandler(
    EventHandler *instance, EventHandlerCallback callback);

PUBLIC void deconstructEventHandler(EventHandler *instance);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __EVENT_HANDLER_H__
