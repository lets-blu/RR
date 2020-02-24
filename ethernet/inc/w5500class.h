#ifndef __W5500CLASS_H__
#define __W5500CLASS_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "assert.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "string.h"

#include "ut.h"
#include "ethernet.h"
#include "gpiopin.h"
#include "keywords.h"

#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_spi.h"

#include "dhcp.h"
#include "socket.h"
#include "w5500.h"

typedef struct {
    struct Ethernet base;
    osThreadId _syncThread;
} W5500Class;

// Constructor
PUBLIC W5500Class newW5500(SPI_TypeDef * SPIx);

// Public method(s)
PUBLIC bool isW5500SyncEnabled(W5500Class * pThis);
PUBLIC void setW5500SyncEnabled(W5500Class * pThis, bool enabled);

PUBLIC STATIC void vSyncW5500Thread(const void * parameter);
PUBLIC STATIC void vW5500TimerThread(const void * parameter);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __W5500CLASS_H__
