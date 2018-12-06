#include "watchdog.h"

PUBLIC Watchdog newWatchdog(uint16_t timeout) {
	// Tout = (4 * (2 ^ prescaler) * reload) / 40
	Watchdog watchdog = {
		._prescaler			= 4, 
		._reload				= 625 * (timeout / 1000), 
		
		.init						= defaultWatchdogInit, 
		.reloadCounter	= defaultReloadWatchdogCounter
	};
	
	return watchdog;
}

PUBLIC VIRTUAL void defaultWatchdogInit(Watchdog * this) {
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(this->_prescaler);
	IWDG_SetReload(this->_reload);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

PUBLIC VIRTUAL void defaultReloadWatchdogCounter(Watchdog * this) {
	IWDG_ReloadCounter();
}
