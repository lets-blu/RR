#include "watchdog.h"

// (de)constructor
PRIVATE WatchDog newWatchDog(uint8_t IWDG_Prescaler, uint16_t reload);

// virtual method
PUBLIC VIRTUAL void initIWDG(WatchDog * this);
PUBLIC VIRTUAL void reloadIWDGCounter(WatchDog * this);

// static member
PUBLIC STATIC WatchDog watchDog = {.reloadCounter = NULL};

PUBLIC void staticWatchDog(void) {
    watchDog = newWatchDog(IWDG_Prescaler_256, 781);
	watchDog._init(&watchDog);
}

PRIVATE WatchDog newWatchDog(uint8_t IWDG_Prescaler, uint16_t reload) {
    WatchDog wacthDog = {
        ._prescaler     = IWDG_Prescaler    ,
        ._reload        = reload            , 
        ._init          = initIWDG          , 
        .reloadCounter  = reloadIWDGCounter
    };

    return wacthDog;
}

PUBLIC VIRTUAL void initIWDG(WatchDog * this) {
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(this->_prescaler);
    IWDG_SetReload(this->_reload);
    IWDG_ReloadCounter();
    IWDG_Enable();
}

PUBLIC VIRTUAL void reloadIWDGCounter(WatchDog * this) {
    IWDG_ReloadCounter();
}
