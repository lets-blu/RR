#ifndef __STANDARD_LOG_PRINTER_H__
#define __STANDARD_LOG_PRINTER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdio.h>
#include <string.h>

#include "resource/inc/IResource.h"
#include "syslog/inc/ILogPrinter.h"
#include "utility/inc/Keywords.h"

typedef struct {
    struct ILogPrinter printer;
    struct IResource *_resource;
} StandardLogPrinter;

// (De)constructors
PUBLIC void deconstructStandardLogPrinter(StandardLogPrinter *instance);

// Public method(s)
PUBLIC void setResourceToStandardLogPrinter(
    StandardLogPrinter *pThis,
    struct IResource *resource);

PUBLIC size_t printByStandardLogPrinter(
    StandardLogPrinter *pThis,
    uint8_t *buffer,
    size_t size);

PUBLIC STATIC StandardLogPrinter *instanceOfStandardLogPrinter(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __STANDARD_LOG_PRINTER_H__
