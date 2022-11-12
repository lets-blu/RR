#include "syslog/inc/LogFilter.h"

PUBLIC void constructLogFilter(
    LogFilter *instance,
    const char *name,
    LogLevel filterLevel)
{
    instance->_name = name;
    instance->_filterLevel = filterLevel;
    instance->_printer = NULL;
}

PUBLIC void deconstructLogFilter(LogFilter *instance)
{
    memset(instance, 0, sizeof(LogFilter));
}

PUBLIC size_t printStringLogByLogFilter(
    LogFilter *pThis,
    LogLevel logLevel,
    const char *format,
    ...)
{
    va_list ap;
    struct ILogPrinter *printer = NULL;
    char buffer[MAX_LOG_FILTER_STRING_LENGTH] = {0};

    // 1. Check parameters
    if (pThis == NULL || format == NULL)
    {
        return 0;
    }

    // 2. Check filter status
    if (pThis->_name == NULL || logLevel < pThis->_filterLevel)
    {
        return 0;
    }

    // 3. Get printer
    if (pThis->_printer != NULL)
    {
        printer = pThis->_printer;
    }
    else
    {
        printer = getDefaultPrinterFromLogPrinterManager(
            instanceOfLogPrinterManager());
    }

    // 4. Format string and print
    if (printer == NULL)
    {
        return 0;
    }

    va_start(ap, format);
    vsnprintf(buffer, MAX_LOG_FILTER_STRING_LENGTH, format, ap);
    va_end(ap);

    return printer->print(printer, (uint8_t *)buffer, strlen(buffer));
}

PUBLIC void setPrinterToLogFilter(LogFilter *pThis, struct ILogPrinter *printer)
{
    if (pThis != NULL)
    {
        pThis->_printer = printer;
    }
}
