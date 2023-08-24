#ifndef __DEVICE_MANAGER_H__
#define __DEVICE_MANAGER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "core/device/inc/DevicePool.h"

#include "port/common/inc/BaseFactory.h"
#include "port/common/inc/BasePin.h"

#define DEVICE_MANAGER_DIGITAL_PIN  BASE_FACTORY_DIGITAL_PIN
#define DEVICE_MANAGER_ANALOG_PIN   BASE_FACTORY_ANALOG_PIN
#define DEVICE_MANAGER_ADDRESS_PIN  BASE_FACTORY_ADDRESS_PIN

typedef struct {
    bool _isConstructed;
    BaseFactory *_factory;
    DevicePool *_pinPool;
} DeviceManager;

typedef BaseFactoryPinType DeviceManagerPinType;

// (De)constructors
PUBLIC void deconstructDeviceManager(DeviceManager *instance);

// Public method(s)
PUBLIC void setFactoryToDeviceManager(
    DeviceManager *pThis, BaseFactory *factory);

PUBLIC void setPinPoolToDeviceManager(
    DeviceManager *pThis, DevicePool *pool);

PUBLIC BasePin *createPinByDeviceManager(
    DeviceManager *pThis,
    DeviceManagerPinType type,
    BasePinParameter *parameter);

PUBLIC void destoryPinByDeviceManager(
    DeviceManager *pThis, DeviceManagerPinType type, BasePin *instance);

PUBLIC STATIC DeviceManager *instanceOfDeviceManager(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DEVICE_MANAGER_H__

