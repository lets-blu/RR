#ifndef __MUTEX_RESOURCE_H__
#define __MUTEX_RESOURCE_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <string.h>

#if defined(ARDUINO_ARCH_AVR)
#include "Arduino_FreeRTOS.h"
#elif defined(ARDUINO_ARCH_STM32)
#include "STM32FreeRTOS.h"
#else
#include "FreeRTOS.h"
#endif

#include "semphr.h"
#include "resource/inc/IResource.h"
#include "utility/inc/Keywords.h"

typedef struct {
    struct IResource resource;
    SemaphoreHandle_t _handle;
} MutexResource;

// (De)constructors
PUBLIC void constructMutexResource(MutexResource *instance);
PUBLIC void deconstructMutexResource(MutexResource *instance);

// Public method(s)
PUBLIC ResourceState takeMutexResource(
    MutexResource *pThis,
    struct IResourceUser *user);

PUBLIC ResourceState giveMutexResource(
    MutexResource *pThis,
    struct IResourceUser *user);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __MUTEX_RESOURCE_H__
