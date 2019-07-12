#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "math.h"

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_iwdg.h"

#include "keywords.h"

typedef struct
{
    IWDG_HandleTypeDef _iwdg;
} Watchdog;

// constructor
PUBLIC Watchdog newWatchdog(void);

// public method(s)
PUBLIC void enableWatchdog(Watchdog * pThis, uint16_t timeout);
PUBLIC void reloadWatchdog(Watchdog * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __WATCHDOG_H__
