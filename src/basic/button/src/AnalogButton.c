#include "basic/button/inc/AnalogButton.h"

// Method implement(s)
PUBLIC void constructAnalogButton(
    AnalogButton *instance, unsigned int startValue, unsigned int endValue)
{
    if (instance == NULL || startValue > endValue) {
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
    if (pThis != NULL) {
        addClickHandlerToBaseButton(&pThis->baseButton, handler);
    }
}

PUBLIC void removeClickHandlerFromAnalogButton(
    AnalogButton *pThis, EventHandler *handler)
{
    if (pThis != NULL) {
        removeClickHandlerFromBaseButton(&pThis->baseButton, handler);
    }
}

PUBLIC void notifyValueChangeToAnalogButton(
    AnalogButton *pThis, unsigned int value)
{
    const ButtonState *currentState = NULL;

    if (pThis == NULL) {
        return;
    }

    currentState = getStateFromBaseButton(&pThis->baseButton);

    if (pThis->_startValue <= value && value <= pThis->_endValue) {
        onPushOnButtonState((ButtonState *)currentState, &pThis->baseButton);
    } else {
        onReleaseOnButtonState((ButtonState *)currentState, &pThis->baseButton);
    }
}

