#include "watchdog.h"

#define __DEFAULT_PRESCALER 4

// type defination
struct Watchdog {
    uint8_t		_prescaler;
    uint16_t	_reload;
};

// static member(s)
PRIVATE STATIC Watchdog watchdog;

// (de)constructor(s)
PRIVATE Watchdog newWatchdog(void) {
    Watchdog watchdog = {
        ._prescaler = 0,
        ._reload = 0
    };

    return watchdog;
}

// public method(s)
PUBLIC void initWatchdog(Watchdog * pThis) {
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(pThis->_prescaler);
    IWDG_SetReload(pThis->_reload);
    IWDG_ReloadCounter();
    IWDG_Enable();
}

PUBLIC void reloadWatchdogCounter(Watchdog * pThis) {
    IWDG_ReloadCounter();
}

PUBLIC void setWatchdogTimeout(Watchdog * pThis, uint16_t timeout) {
    pThis->_prescaler = __DEFAULT_PRESCALER;
    pThis->_reload = 625 * (timeout / 1000);
}

PUBLIC STATIC Watchdog * getWatchdogInstance(void) {
    static Watchdog * pWatchdog = NULL;

    if (pWatchdog == NULL) {
        watchdog = newWatchdog();
        pWatchdog = &watchdog;
    }

    return pWatchdog;
}
