#ifndef __LOG_PRINTER_H__
#define __LOG_PRINTER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <stdint.h>

struct ILogPrinter {
    size_t (*print)(struct ILogPrinter *pThis, uint8_t *buffer, size_t size);
};

typedef size_t (*ILogPrinterPrintMethod)(struct ILogPrinter *, uint8_t *, size_t);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LOG_PRINTER_H__
