#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include "stdlib.h"
#include "stm32f10x.h"
#include "stm32f10x_iwdg.h"

#include "keywords.h"

#define RELOAD_WATCHDOG() reloadWatchdogCounter(getWatchdogInstance())

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	
	struct Watchdog;
	typedef struct Watchdog Watchdog;
	
	// public method(s)
	PUBLIC void initWatchdog(Watchdog * pThis);
	PUBLIC void reloadWatchdogCounter(Watchdog * pThis);
	PUBLIC void setWatchdogTimeout(Watchdog * pThis, uint16_t timeout);
	
	PUBLIC STATIC Watchdog * getWatchdogInstance(void);
	
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __WATCHDOG_H__
