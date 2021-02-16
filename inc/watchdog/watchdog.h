#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "math.h"
#include "stdint.h"
#include "keywords.h"

#ifdef UNIT_TEST
#include "hal.h"
#include "hal_iwdg.h"
#else
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_iwdg.h"
#endif // UNIT_TEST

typedef struct {
    IWDG_HandleTypeDef _iwdg;
} Watchdog;

// Constructor
PUBLIC Watchdog newWatchdog(uint16_t counter);

// Public method(s)
PUBLIC void enableWatchdog(Watchdog * pThis);
PUBLIC void reloadWatchdog(Watchdog * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __WATCHDOG_H__
