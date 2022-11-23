#include "resource/inc/MutexResource.h"

PUBLIC void constructMutexResource(MutexResource *instance)
{
    instance->resource.take = (IResourceTakeMethod)takeMutexResource;
    instance->resource.give = (IResourceGiveMethod)giveMutexResource;
    instance->_handle = xSemaphoreCreateMutex();
}

PUBLIC void deconstructMutexResource(MutexResource *instance)
{
    vSemaphoreDelete(instance->_handle);
    memset(instance, 0, sizeof(MutexResource));
}

PUBLIC ResourceState takeMutexResource(
    MutexResource *pThis,
    struct IResourceUser *user)
{
    (void)user;

    if (pThis == NULL)
    {
        return RESOURCE_UNAVAILANLE;
    }

    xSemaphoreTake(pThis->_handle, portMAX_DELAY);

    return RESOURCE_AVAILABLE;
}

PUBLIC ResourceState giveMutexResource(
    MutexResource *pThis,
    struct IResourceUser *user)
{
    (void)user;

    if (pThis == NULL)
    {
        return RESOURCE_AVAILABLE;
    }

    xSemaphoreGive(pThis->_handle);

    return RESOURCE_UNAVAILANLE;
}
