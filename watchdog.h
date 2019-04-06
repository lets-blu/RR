#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "math.h"
#include "stdlib.h"

#include "stm32f10x.h"
#include "stm32f10x_iwdg.h"

#include "keywords.h"

#define RELOAD_WATCHDOG() reloadWatchdogCounter(getWatchdogInstance())

    struct Watchdog;
    typedef struct Watchdog Watchdog;

    // public method(s)
    PUBLIC void initWatchdog(Watchdog *pThis, uint16_t timeout);
    PUBLIC void reloadWatchdogCounter(Watchdog *pThis);

    PUBLIC STATIC Watchdog *getWatchdogInstance(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __WATCHDOG_H__
