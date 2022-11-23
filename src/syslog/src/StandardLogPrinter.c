#include "syslog/inc/StandardLogPrinter.h"

#define IS_STANDARD_LOG_PRINTER_CONSTRUCTED(instance) \
    ((instance)->printer.print != NULL)

// Private member(s)
PRIVATE STATIC StandardLogPrinter instance;

PRIVATE void constructStandardLogPrinter(StandardLogPrinter *instance)
{
    instance->printer.print = (ILogPrinterPrintMethod)printByStandardLogPrinter;
    instance->_resource = NULL;
}

PUBLIC void deconstructStandardLogPrinter(StandardLogPrinter *instance)
{
    memset(instance, 0, sizeof(StandardLogPrinter));
}

PUBLIC void setResourceToStandardLogPrinter(
    StandardLogPrinter *pThis,
    struct IResource *resource)
{
    if (pThis != NULL)
    {
        pThis->_resource = resource;
    }
}

PUBLIC size_t printByStandardLogPrinter(
    StandardLogPrinter *pThis,
    uint8_t *buffer,
    size_t size)
{
    if (pThis == NULL || buffer == NULL)
    {
        return 0;
    }

    if (pThis->_resource != NULL)
    {
        pThis->_resource->take(pThis->_resource, NULL);
    }

    for (size_t i = 0; i < size; i++)
    {
        putchar(buffer[i]);
    }

    if (pThis->_resource != NULL)
    {
        pThis->_resource->give(pThis->_resource, NULL);
    }

    return size;
}

PUBLIC STATIC StandardLogPrinter *instanceOfStandardLogPrinter(void)
{
    if (!IS_STANDARD_LOG_PRINTER_CONSTRUCTED(&instance))
    {
        constructStandardLogPrinter(&instance);
    }

    return &instance;
}
