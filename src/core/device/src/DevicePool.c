#include "core/device/inc/DevicePool.h"

// Method implement(s)
PUBLIC void constructDevicePool(
    DevicePool *instance, unsigned int number, unsigned int size)
{
    BaseDevice *device = NULL;

    if (instance == NULL || number * size == 0) {
        return;
    }

    // 1. Initialize buffer
    instance->_base = calloc(number, size);

    if (instance->_base == NULL) {
        return;
    }

    // 2. Add to free list
    constructLinkedList(&instance->_freeList);

    for (unsigned int i = 0; i < number; i++) {
        device = (BaseDevice *)(instance->_base + i * size);
        addItemToLinkedList(&instance->_freeList, &device->base);
    }
}

PUBLIC void deconstructDevicePool(DevicePool *instance)
{
    if (instance == NULL) {
        return;
    }

    if (instance->_base != NULL) {
        free(instance->_base);
    }

    deconstructLinkedList(&instance->_freeList);
    memset(instance, 0, sizeof(DevicePool));
}

PUBLIC BaseDevice *allocateFromDevicePool(DevicePool *pThis)
{
    if (pThis == NULL) {
        return NULL;
    }

    return LinkedListItem2BaseDevice(
        removeIndexFromLinkedList(&pThis->_freeList, 0));
}

PUBLIC void freeToDevicePool(DevicePool *pThis, BaseDevice *device)
{
    if (pThis != NULL && device != NULL) {
        addItemToLinkedList(&pThis->_freeList, &device->base);
    }
}

