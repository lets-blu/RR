#include "watchdog.h"

#define WDT_INSTANCE    IWDG
#define WDT_PRESCALER   IWDG_PRESCALER_64

PUBLIC Watchdog newWatchdog(uint16_t counter)
{
    Watchdog watchdog = {
        ._iwdg = {
            .Instance = WDT_INSTANCE, 
            .Init = {
                .Prescaler  = WDT_PRESCALER,
                .Reload     = (uint16_t)(10 * counter / pow(2, WDT_PRESCALER))
            }
        }
    };

    return watchdog;
}

PUBLIC void enableWatchdog(Watchdog * pThis)
{
    HAL_IWDG_Init(&pThis->_iwdg);
}

PUBLIC void reloadWatchdog(Watchdog * pThis)
{
    HAL_IWDG_Refresh(&pThis->_iwdg);
}
