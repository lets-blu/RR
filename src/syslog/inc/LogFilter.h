#ifndef __LOG_FILTER_H__
#define __LOG_FILTER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "syslog/inc/ILogPrinter.h"
#include "syslog/inc/LogPrinterManager.h"
#include "utility/inc/Keywords.h"

#define MAX_LOG_FILTER_STRING_LENGTH        80

#define STATIC_LOG_FILTER(name, level) {    \
    ._name          = (name),               \
    ._filterLevel   = (level),              \
    ._printer       = NULL                  \
}

#define LOG_I(filter, format, args...)      \
    printStringLogByLogFilter(              \
        (filter),                           \
        LOG_LEVEL_INFO,                     \
        "[%s][I] " format "\r\n",           \
        (filter)->_name, ##args)

#define LOG_W(filter, format, args...)      \
    printStringLogByLogFilter(              \
        (filter),                           \
        LOG_LEVEL_WARNING,                  \
        "[%s][W] " format "\r\n",           \
        (filter)->_name, ##args)

#define LOG_E(filter, format, args...)      \
    printStringLogByLogFilter(              \
        (filter),                           \
        LOG_LEVEL_ERROR,                    \
        "[%s][E] " format "\r\n",           \
        (filter)->_name, ##args)

typedef enum {
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_OFF
} LogLevel;

typedef struct {
    const char *_name;
    LogLevel _filterLevel;
    struct ILogPrinter *_printer;
} LogFilter;

// (De)constructors
PUBLIC void constructLogFilter(
    LogFilter *instance,
    const char *name,
    LogLevel filterLevel);

PUBLIC void deconstructLogFilter(LogFilter *instance);

// Public method(s)
PUBLIC size_t printStringLogByLogFilter(
    LogFilter *pThis,
    LogLevel logLevel,
    const char *format,
    ...);

PUBLIC void setPrinterToLogFilter(LogFilter *pThis, struct ILogPrinter *printer);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LOG_FILTER_H__
