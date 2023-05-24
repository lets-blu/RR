#include "basic/button/inc/BaseButton.h"
#include "basic/button/inc/ButtonState.h"

// Private member(s)
PRIVATE STATIC LogFilter filter
    = STATIC_LOG_FILTER("BaseButton", LOG_LEVEL_INFO);

// Protected method(s)
PROTECTED void constructBaseButton(BaseButton *instance);
PROTECTED void deconstructBaseButton(BaseButton *instance);

// Method implement(s)
PROTECTED void constructBaseButton(BaseButton *instance)
{
    if (instance != NULL) {
        instance->_currentState = BUTTON_STATE_RELEASED;
        constructLinkedList(&instance->_clickHandlers);
    }
}

PROTECTED void deconstructBaseButton(BaseButton *instance)
{
    if (instance != NULL) {
        deconstructLinkedList(&instance->_clickHandlers);
        memset(instance, 0, sizeof(BaseButton));
    }
}

PUBLIC void addClickHandlerToBaseButton(
    BaseButton *pThis, EventHandler *handler)
{
    if (pThis != NULL && handler != NULL) {
        addItemToLinkedList(&pThis->_clickHandlers, &handler->base);
    }
}

PUBLIC void removeClickHandlerFromBaseButton(
    BaseButton *pThis, EventHandler *handler)
{
    if (pThis != NULL && handler != NULL) {
        removeItemFromLinkedList(&pThis->_clickHandlers, &handler->base);
    }
}

PUBLIC void setStateToBaseButton(
    BaseButton *pThis, const struct IButtonState *state)
{
    const IButtonState *previousState = NULL;

    if (pThis == NULL || state == NULL) {
        return;
    }

    previousState = pThis->_currentState;
    pThis->_currentState = state;

    LOG_I(&filter, "0x%x setState, %s -> %s",
        pThis,
        previousState->vtbl->toString((IButtonState *)previousState),
        state->vtbl->toString((IButtonState *)state));
}

PUBLIC const struct IButtonState *getStateFromBaseButton(BaseButton *pThis)
{
    return (pThis == NULL) ? NULL : pThis->_currentState;
}

PUBLIC void notifyClickToBaseButton(BaseButton *pThis)
{
    LinkedListIterator iterator;
    EventHandler *handler = NULL;

    if (pThis == NULL) {
        return;
    }

    constructLinkedListIterator(&iterator, &pThis->_clickHandlers);

    while (hasNextOfLinkedListIterator(&iterator)) {
        handler = LinkedListItem2EventHandler(nextOfLinkedListIterator(&iterator));
        handler->callback(handler, pThis, NULL);
    }

    deconstructLinkedListIterator(&iterator);
}

