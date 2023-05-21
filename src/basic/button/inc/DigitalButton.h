#ifndef __DIGITAL_BUTTON_H__
#define __DIGITAL_BUTTON_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "basic/button/inc/BaseButton.h"
#include "basic/button/inc/ButtonState.h"

#include "core/common/inc/Keywords.h"
#include "core/device/inc/BaseScannable.h"
#include "core/device/inc/DeviceManager.h"
#include "core/event/inc/EventHandler.h"
#include "core/list/inc/LinkedList.h"

#include "port/common/inc/BasePin.h"

#define BaseButton2DigitalButton(instance)      \
    BASE_TO_SUB_CAST(instance, DigitalButton, baseButton)

#define BaseScannable2DigitalButton(instance)   \
    BASE_TO_SUB_CAST(instance, DigitalButton, baseScannable)

typedef struct {
    BaseButton baseButton;
    BaseScannable baseScannable;

    BasePin *_basePin;
    PinState _pushState;

    LinkedList _pushHandlers;
    LinkedList _releaseHandlers;
} DigitalButton;

// (De)constructors
PUBLIC void constructDigitalButton(
    DigitalButton *instance, void *port, unsigned int pin, PinState pushState);

PUBLIC void deconstructDigitalButton(DigitalButton *instance);

// Public method(s)
PUBLIC void addPushHandlerToDigitalButton(
    DigitalButton *pThis, EventHandler *handler);

PUBLIC void removePushHandlerFromDigitalButton(
    DigitalButton *pThis, EventHandler *handler);

PUBLIC void addReleaseHandlerToDigitalButton(
    DigitalButton *pThis, EventHandler *handler);

PUBLIC void removeReleaseHandlerFromDigitalButton(
    DigitalButton *pThis, EventHandler *handler);

PUBLIC void scanDigitalButton(DigitalButton *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DIGITAL_BUTTON_H__

