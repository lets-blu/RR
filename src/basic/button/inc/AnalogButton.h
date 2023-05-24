#ifndef __ANALOG_BUTTON_H__
#define __ANALOG_BUTTON_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "basic/button/inc/BaseButton.h"
#include "basic/button/inc/ButtonState.h"

#include "core/common/inc/Keywords.h"
#include "core/event/inc/EventHandler.h"

#define BaseButton2AnalogButton(instance)       \
    BASE_TO_SUB_CAST(instance, AnalogButton, baseButton)

#define LinkedListItem2AnalogButton(instance)   \
    BASE_TO_SUB_CAST(instance, AnalogButton, baseItem)

typedef struct {
    BaseButton baseButton;
    LinkedListItem baseItem;

    unsigned int _startValue;
    unsigned int _endValue;
} AnalogButton;

// (De)constructors
PUBLIC void constructAnalogButton(
    AnalogButton *instance, unsigned int startValue, unsigned int endValue);

PUBLIC void deconstructAnalogButton(AnalogButton *instance);

// Public method(s)
PUBLIC void addClickHandlerToAnalogButton(
    AnalogButton *pThis, EventHandler *handler);

PUBLIC void removeClickHandlerFromAnalogButton(
    AnalogButton *pThis, EventHandler *handler);

PUBLIC void notifyValueChangeToAnalogButton(
    AnalogButton *pThis, unsigned int value);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ANALOG_BUTTON_H__

