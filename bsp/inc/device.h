#ifndef __DEVICE_H__
#define __DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdbool.h"
#include "stddef.h"
#include "string.h"

#include "led.h"
#include "button.h"
#include "shiftregister.h"
#include "registergroup.h"
#include "dataselector.h"
#include "selectorgroup.h"
#include "watchdog.h"

#include "keywords.h"
#include "chainedobserver.h"

typedef void Device;

// Public method(s)
PUBLIC STATIC void initializeAllDevices(void);

PUBLIC STATIC LED * getLEDDevice(const char * name);
PUBLIC STATIC Button * getButtonDevice(const char * name);
PUBLIC STATIC RegisterGroup * getRegisterGroupDevice(void);
PUBLIC STATIC SelectorGroup * getSelectorGroupDevice(void);

PUBLIC STATIC void enableWatchdogDevice(uint16_t timeout);
PUBLIC STATIC void reloadWatchdogDevice(void);

PUBLIC STATIC void setButtonDeviceClickCallback(Button * button, 
    on_button_click_fp callback);
PUBLIC STATIC bool addSelectorGroupDeviceScanCallback(
    SelectorGroup * selectorGroup, update_observer_fp callback);

#ifdef __cplusplus
}
#endif // __cplsuplus

#endif // __DEVICE_H__
