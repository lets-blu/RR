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

#include "port/common/inc/BasePin.h"

#define BaseButton2DigitalButton(instance)      \
    BASE_TO_SUB_CAST(instance, DigitalButton, baseButton)

#define BaseScannable2DigitalButton(instance)   \
    BASE_TO_SUB_CAST(instance, DigitalButton, baseScannable)

typedef struct {
    BaseButton baseButton;
    BaseScannable baseScannable;

    BasePin *_pin;
    unsigned int _pushState;
} DigitalButton;

// (De)constructors
PUBLIC void constructDigitalButton(
    DigitalButton *instance,
    BasePinParameter *parameter,
    unsigned int pushState);

PUBLIC void deconstructDigitalButton(DigitalButton *instance);

// Public method(s)
PUBLIC void addClickHandlerToDigitalButton(
    DigitalButton *pThis, EventHandler *handler);

PUBLIC void removeClickHandlerFromDigitalButton(
    DigitalButton *pThis, EventHandler *handler);

PUBLIC void scanDigitalButton(DigitalButton *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DIGITAL_BUTTON_H__

