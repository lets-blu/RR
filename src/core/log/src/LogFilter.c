#include "core/log/inc/LogFilter.h"

// Method implement(s)
PUBLIC void constructLogFilter(
    LogFilter *instance, const char *name, LogLevel filterLevel)
{
    if (instance != NULL) {
        instance->_name = name;
        instance->_filterLevel = filterLevel;
    }
}

PUBLIC void deconstructLogFilter(LogFilter *instance)
{
    if (instance != NULL) {
        memset(instance, 0, sizeof(LogFilter));
    }
}

PUBLIC int printStringByLogFilter(
    LogFilter *pThis, LogLevel logLevel, const char *format, ...)
{
    va_list ap;
    int length = 0;

    // 1. Check arguments
    if (pThis == NULL || format == NULL) {
        return 0;
    }

    // 2. Check filter status
    if (pThis->_name == NULL || pThis->_filterLevel > logLevel) {
        return 0;
    }

    // 3. Print fomatted string
    va_start(ap, format);
    length = vprintf(format, ap);
    va_end(ap);

    return length;
}

