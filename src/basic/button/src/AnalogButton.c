#include "basic/button/inc/AnalogButton.h"

// Protected method(s)
PROTECTED void constructBaseButton(BaseButton *instance);
PROTECTED void deconstructBaseButton(BaseButton *instance);

// Override method(s)
PUBLIC OVERRIDE void notifyPushByAnalogButtonBaseButton(BaseButton *button);
PUBLIC OVERRIDE void notifyReleaseByAnalogButtonBaseButton(BaseButton *button);

// Virtual methods table
static const BaseButtonVtbl buttonVtbl = {
    .notifyPush     = notifyPushByAnalogButtonBaseButton,
    .notifyRelease  = notifyReleaseByAnalogButtonBaseButton
};

// Method implement(s)
PUBLIC void constructAnalogButton(
    AnalogButton *instance, unsigned int startValue, unsigned int endValue)
{
    if (instance == NULL) {
        return;
    }

    // 1. construct base
    constructBaseButton(&instance->baseButton);
    instance->baseButton.vtbl = &buttonVtbl;

    constructLinkedListItem(&instance->baseItem);

    // 2. construct member(s)
    instance->_startValue = startValue;
    instance->_endValue = endValue;

    constructLinkedList(&instance->_pushHandlers);
    constructLinkedList(&instance->_releaseHandlers);
}

PUBLIC void deconstructAnalogButton(AnalogButton *instance)
{
    if (instance == NULL) {
        return;
    }

    // 1. deconstruct base
    deconstructBaseButton(&instance->baseButton);
    deconstructLinkedListItem(&instance->baseItem);

    // 2. deconstruct member(s)
    deconstructLinkedList(&instance->_pushHandlers);
    deconstructLinkedList(&instance->_releaseHandlers);

    memset(instance, 0, sizeof(AnalogButton));
}

PUBLIC void addPushHandlerToAnalogButton(
    AnalogButton *pThis, EventHandler *handler)
{
    if (pThis != NULL && handler != NULL) {
        addItemToLinkedList(&pThis->_pushHandlers, &handler->base);
    }
}

PUBLIC void removePushHandlerFromAnalogButton(
    AnalogButton *pThis, EventHandler *handler)
{
    if (pThis != NULL && handler != NULL) {
        removeItemFromLinkedList(&pThis->_pushHandlers, &handler->base);
    }
}

PUBLIC void addReleaseHandlerToAnalogButton(
    AnalogButton *pThis, EventHandler *handler)
{
    if (pThis != NULL && handler != NULL) {
        addItemToLinkedList(&pThis->_releaseHandlers, &handler->base);
    }
}

PUBLIC void removeReleaseHandlerFromAnalogButton(
    AnalogButton *pThis, EventHandler *handler)
{
    if (pThis != NULL && handler != NULL) {
        removeItemFromLinkedList(&pThis->_releaseHandlers, &handler->base);
    }
}

PUBLIC void notifyValueChangeToAnalogButton(
    AnalogButton *pThis, unsigned int value)
{
    const IButtonState *currentState = NULL;

    if (pThis == NULL) {
        return;
    }

    currentState = getStateFromBaseButton(&pThis->baseButton);

    if (pThis->_startValue <= value && value <= pThis->_endValue) {
        currentState->vtbl->onPush(
            (IButtonState *)currentState, &pThis->baseButton);
    } else {
        currentState->vtbl->onRelease(
            (IButtonState *)currentState, &pThis->baseButton);
    }
}

PUBLIC OVERRIDE void notifyPushByAnalogButtonBaseButton(BaseButton *button)
{
    LinkedListIterator iterator;
    EventHandler *handler = NULL;
    AnalogButton *pThis = BaseButton2AnalogButton(button);

    if (pThis == NULL) {
        return;
    }

    constructLinkedListIterator(&iterator, &pThis->_pushHandlers);

    while (hasNextOfLinkedListIterator(&iterator)) {
        handler = LinkedListItem2EventHandler(nextOfLinkedListIterator(&iterator));
        handler->callback(handler, pThis, NULL);
    }

    deconstructLinkedListIterator(&iterator);
}

PUBLIC OVERRIDE void notifyReleaseByAnalogButtonBaseButton(BaseButton *button)
{
    LinkedListIterator iterator;
    EventHandler *handler = NULL;
    AnalogButton *pThis = BaseButton2AnalogButton(button);

    if (pThis == NULL) {
        return;
    }

    constructLinkedListIterator(&iterator, &pThis->_releaseHandlers);

    while (hasNextOfLinkedListIterator(&iterator)) {
        handler = LinkedListItem2EventHandler(nextOfLinkedListIterator(&iterator));
        handler->callback(handler, pThis, NULL);
    }

    deconstructLinkedListIterator(&iterator);
}

