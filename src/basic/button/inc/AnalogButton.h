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
#include "core/list/inc/LinkedList.h"

#define BaseButton2AnalogButton(instance)       \
    BASE_TO_SUB_CAST(instance, AnalogButton, baseButton)

#define LinkedListItem2AnalogButton(instance)   \
    BASE_TO_SUB_CAST(instance, AnalogButton, baseItem)

typedef struct {
    BaseButton baseButton;
    LinkedListItem baseItem;

    unsigned int _startValue;
    unsigned int _endValue;

    LinkedList _pushHandlers;
    LinkedList _releaseHandlers;
} AnalogButton;

// (De)constructors
PUBLIC void constructAnalogButton(
    AnalogButton *instance, unsigned int startValue, unsigned int endValue);

PUBLIC void deconstructAnalogButton(AnalogButton *instance);

// Public method(s)
PUBLIC void addPushHandlerToAnalogButton(
    AnalogButton *pThis, EventHandler *handler);

PUBLIC void removePushHandlerFromAnalogButton(
    AnalogButton *pThis, EventHandler *handler);

PUBLIC void addReleaseHandlerToAnalogButton(
    AnalogButton *pThis, EventHandler *handler);

PUBLIC void removeReleaseHandlerFromAnalogButton(
    AnalogButton *pThis, EventHandler *handler);

PUBLIC void notifyValueChangeToAnalogButton(
    AnalogButton *pThis, unsigned int value);

PUBLIC void scanAnalogButton(AnalogButton *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ANALOG_BUTTON_H__

