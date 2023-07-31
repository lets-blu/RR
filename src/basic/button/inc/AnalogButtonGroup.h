#ifndef __ANALOG_BUTTON_GROUP_H__
#define __ANALOG_BUTTON_GROUP_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "basic/button/inc/AnalogButton.h"

#include "core/common/inc/Keywords.h"
#include "core/device/inc/BaseScannable.h"
#include "core/device/inc/DeviceManager.h"
#include "core/list/inc/LinkedList.h"

#include "port/common/inc/BasePin.h"

#define BaseScannable2AnalogButtonGroup(instance) \
    BASE_TO_SUB_CAST(instance, AnalogButtonGroup, base)

typedef struct {
    BaseScannable base;

    BasePin *_pin;
    LinkedList _analogButtons;
} AnalogButtonGroup;

// (De)constructors
PUBLIC void constructAnalogButtonGroup(
    AnalogButtonGroup *instance, BasePinParameter *parameter);

PUBLIC void deconstructAnalogButtonGroup(AnalogButtonGroup *instance);

// Public method(s)
PUBLIC void addButtonToAnalogButtonGroup(
    AnalogButtonGroup *pThis, AnalogButton *button);

PUBLIC void removeButtonFromAnalogButtonGroup(
    AnalogButtonGroup *pThis, AnalogButton *button);

PUBLIC void scanAnalogButtonGroup(AnalogButtonGroup *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ANALOG_BUTTON_GROUP_H__

