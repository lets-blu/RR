#include "core/event/inc/EventHandler.h"

// Method implement(s)
PUBLIC void constructEventHandler(
    EventHandler *instance, EventHandlerCallback callback)
{
    if (instance != NULL) {
        constructLinkedListItem(&instance->base);
        instance->callback = callback;
    }
}

PUBLIC void deconstructEventHandler(EventHandler *instance)
{
    if (instance != NULL) {
        deconstructLinkedListItem(&instance->base);
        memset(instance, 0, sizeof(EventHandler));
    }
}
