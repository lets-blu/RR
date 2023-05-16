#ifndef __BASE_DEVICE_H__
#define __BASE_DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "core/list/inc/LinkedList.h"

#define LinkedListItem2BaseDevice(instance) \
    BASE_TO_SUB_CAST(instance, BaseDevice, base)

typedef struct {
    LinkedListItem base;
} BaseDevice;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __BASE_DEVICE_H__

