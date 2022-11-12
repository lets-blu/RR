#ifndef __DEFAULT_LOG_PRINTER_H__
#define __DEFAULT_LOG_PRINTER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdio.h>
#include <string.h>

#include "syslog/inc/ILogPrinter.h"
#include "utility/inc/Keywords.h"

typedef struct {
    struct ILogPrinter printer;
} DefaultLogPrinter;

// (De)constructors
PUBLIC void constructDefaultLogPrinter(DefaultLogPrinter *instance);
PUBLIC void deconstructDefaultLogPrinter(DefaultLogPrinter *instance);

// Public method(s)
PUBLIC size_t printByDefaultLogPrinter(
    DefaultLogPrinter *pThis,
    uint8_t *buffer,
    size_t size);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DEFAULT_LOG_PRINTER_H__
