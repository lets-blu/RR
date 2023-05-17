#ifndef __LOG_FILTER_H__
#define __LOG_FILTER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "core/common/inc/Keywords.h"

#define STATIC_LOG_FILTER(name, level) {    \
    ._name          = (name),               \
    ._filterLevel   = (level)               \
}

#define LOG_I(filter, format, args...)      \
    printStringByLogFilter(                 \
        (LogFilter *)(filter),              \
        LOG_LEVEL_INFO,                     \
        "[%s][I] " format "\r\n",           \
        ((LogFilter *)(filter))->_name, ##args)

#define LOG_W(filter, format, args...)      \
    printStringByLogFilter(                 \
        (LogFilter *)(filter),              \
        LOG_LEVEL_WARNING,                  \
        "[%s][W] " format "\r\n",           \
        ((LogFilter *)(filter))->_name, ##args)

#define LOG_E(filter, format, args...)      \
    printStringByLogFilter(                 \
        (LogFilter *)(filter),              \
        LOG_LEVEL_ERROR,                    \
        "[%s][E] " format "\r\n",           \
        ((LogFilter *)(filter))->_name, ##args)

typedef enum {
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_OFF
} LogLevel;

typedef struct {
    const char *_name;
    LogLevel _filterLevel;
} LogFilter;

// (De)constructors
PUBLIC void constructLogFilter(
    LogFilter *instance, const char *name, LogLevel filterLevel);

PUBLIC void deconstructLogFilter(LogFilter *instance);

// Public method(s)
PUBLIC int printStringByLogFilter(
    LogFilter *pThis, LogLevel logLevel, const char *format, ...);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LOG_FILTER_H__

