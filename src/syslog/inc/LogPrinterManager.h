#ifndef __LOG_PRINTER_MANAGER_H__
#define __LOG_PRINTER_MANAGER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "syslog/inc/ILogPrinter.h"
#include "utility/inc/ArrayList.h"
#include "utility/inc/Keywords.h"

#define MAX_LOG_PRINTER_MANAGER_PRINTERS_NUMBER 1

typedef struct {
    struct ILogPrinter *_defaultPrinter;
    ArrayList _printersList;
} LogPrinterManager;

typedef struct {
    ArrayListItem arrayListItem;
    const char *_name;
    struct ILogPrinter *_printer;
} LogPrinterManagerItem;

// Public method(s)
PUBLIC bool addPrinterToLogPrinterManager(
    LogPrinterManager *pThis,
    const char *name,
    struct ILogPrinter *printer);

PUBLIC void removePrinterFromLogPrinterManager(
    LogPrinterManager *pThis,
    const char *name);

PUBLIC void setDefaultPrinterToLogPrinterManager(
    LogPrinterManager *pThis,
    const char *name);

PUBLIC struct ILogPrinter *getDefaultPrinterFromLogPrinterManager(
    LogPrinterManager *pThis);

PUBLIC bool equalsBetweenLogPrinterManagerItem(
    LogPrinterManagerItem *pThis,
    LogPrinterManagerItem *item);

PUBLIC STATIC LogPrinterManager *instanceOfLogPrinterManager(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LOG_PRINTER_MANAGER_H__
