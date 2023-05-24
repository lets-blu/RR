#include "basic/button/inc/AnalogButton.h"

// Protected method(s)
PROTECTED void constructBaseButton(BaseButton *instance);
PROTECTED void deconstructBaseButton(BaseButton *instance);

// Method implement(s)
PUBLIC void constructAnalogButton(
    AnalogButton *instance, unsigned int startValue, unsigned int endValue)
{
    if (instance == NULL) {
        return;
    }

    // 1. construct base
    constructBaseButton(&instance->baseButton);
    constructLinkedListItem(&instance->baseItem);

    // 2. construct member(s)
    instance->_startValue = startValue;
    instance->_endValue = endValue;
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
    memset(instance, 0, sizeof(AnalogButton));
}

PUBLIC void addClickHandlerToAnalogButton(
    AnalogButton *pThis, EventHandler *handler)
{
    if (pThis != NULL && handler != NULL) {
        addClickHandlerToBaseButton(&pThis->baseButton, handler);
    }
}

PUBLIC void removeClickHandlerFromAnalogButton(
    AnalogButton *pThis, EventHandler *handler)
{
    if (pThis != NULL && handler != NULL) {
        removeClickHandlerFromBaseButton(&pThis->baseButton, handler);
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

