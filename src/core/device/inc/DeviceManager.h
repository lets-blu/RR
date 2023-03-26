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

#define IS_DEVICE_MANAGER_CONSTRUCTED(instance) \
    (((DeviceManager *)(instance))->_isConstructed)

typedef struct {
    bool _isConstructed;
    BaseFactory *_factory;
    DevicePool _basePinPool;
} DeviceManager;

// (De)constructors
PUBLIC void deconstructDeviceManager(DeviceManager *instance);

// Public method(s)
PUBLIC BasePin *createBasePinByDeviceManager(
    DeviceManager *pThis, void *port, unsigned int pin);

PUBLIC void destoryBasePinByDeviceManager(
    DeviceManager *pThis, BasePin *instance);

PUBLIC void setFactoryToDeviceManager(
    DeviceManager *pThis, BaseFactory *factory);

PUBLIC void setBasePinToDeviceManager(
    DeviceManager *pThis, unsigned int number, unsigned int size);

PUBLIC STATIC DeviceManager *instanceOfDeviceManager(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DEVICE_MANAGER_H__

