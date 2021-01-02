#include "watchdog.h"

#define WDT_INSTANCE    IWDG
#define WDT_PRESCALER   IWDG_PRESCALER_64

PUBLIC Watchdog newWatchdog(void)
{
    Watchdog watchdog = {
        ._iwdg = {
            .Instance = WDT_INSTANCE, 
            .Init = {
                .Prescaler  = WDT_PRESCALER,
                .Reload     = 0
            }
        }
    };

    return watchdog;
}

PUBLIC void enableWatchdog(Watchdog * pThis, uint16_t timeout)
{
    pThis->_iwdg.Init.Reload = (uint16_t)(10 * timeout / pow(2, WDT_PRESCALER));
    HAL_IWDG_Init(&pThis->_iwdg);
}

PUBLIC void reloadWatchdog(Watchdog * pThis)
{
    HAL_IWDG_Refresh(&pThis->_iwdg);
}
