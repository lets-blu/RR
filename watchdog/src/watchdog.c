#include "watchdog.h"

#define __WDT_INSTANCE  IWDG
#define __WDT_PRESCALER IWDG_PRESCALER_64

PUBLIC Watchdog newWatchdog(void)
{
    Watchdog watchdog = 
    {
        ._iwdg  =
        {
            .Instance       = __WDT_INSTANCE, 

            .Init.Prescaler = __WDT_PRESCALER, 
            .Init.Reload    = 0
        }
    };

    return watchdog;
}

PUBLIC void enableWatchdog(Watchdog * pThis, uint16_t timeout)
{
    pThis->_iwdg.Init.Reload = 10 * timeout / pow(2, pThis->_iwdg.Init.Prescaler);
    HAL_IWDG_Init(&pThis->_iwdg);
}

PUBLIC void reloadWatchdog(Watchdog * pThis)
{
    HAL_IWDG_Refresh(&pThis->_iwdg);
}
