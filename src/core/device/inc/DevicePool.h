#ifndef __DEVICE_POOL_H__
#define __DEVICE_POOL_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "core/device/inc/BaseDevice.h"
#include "core/list/inc/LinkedList.h"

#define IS_DEVICE_POOL_CONSTRUCTED(instance) \
    (((DevicePool *)(instance))->_base != NULL)

typedef struct {
    uint8_t *_base;
    LinkedList _freeList;
} DevicePool;

// (De)constructors
PUBLIC void constructDevicePool(
    DevicePool *instance, unsigned int number, unsigned int size);

PUBLIC void deconstructDevicePool(DevicePool *instance);

// Public method(s)
PUBLIC BaseDevice *allocateFromDevicePool(DevicePool *pThis);
PUBLIC void freeToDevicePool(DevicePool *pThis, BaseDevice *device);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DEVICE_POOL_H__

