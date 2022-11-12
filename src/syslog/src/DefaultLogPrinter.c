#include "syslog/inc/DefaultLogPrinter.h"

PUBLIC void constructDefaultLogPrinter(DefaultLogPrinter *instance)
{
    instance->printer.print = (ILogPrinterPrintMethod)printByDefaultLogPrinter;
}

PUBLIC void deconstructDefaultLogPrinter(DefaultLogPrinter *instance)
{
    memset(instance, 0, sizeof(DefaultLogPrinter));
}

PUBLIC size_t printByDefaultLogPrinter(
    DefaultLogPrinter *pThis,
    uint8_t *buffer,
    size_t size)
{
    if (pThis == NULL || buffer == NULL)
    {
        return 0;
    }

    for (size_t i = 0; i < size; i++)
    {
        putchar(buffer[i]);
    }

    return size;
}
