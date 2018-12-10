#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include "math.h"
#include "stdbool.h"
#include "stm32f10x.h"
#include "stm32f10x_iwdg.h"

#include "keywords.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	typedef struct Watchdog {
		uint8_t	_prescaler;
		uint16_t _reload;
		
		void (*init)(struct Watchdog * this);
		void (*reloadCounter)(struct Watchdog * this);
	} Watchdog;
	
	// (de)constructor(s)
	PUBLIC Watchdog newWatchdog(uint16_t timeout);
	
	// public method(s)
	PUBLIC VIRTUAL void defaultWatchdogInit(Watchdog * this);
	PUBLIC VIRTUAL void defaultReloadWatchdogCounter(Watchdog * this);
	
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __WATCHDOG_H__
