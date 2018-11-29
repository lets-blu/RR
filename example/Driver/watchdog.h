#ifndef __WATHCDOG_H__
#define __WATHCDOG_H__

#include "stdlib.h"
#include "stm32f10x.h"
#include "stm32f10x_iwdg.h"

#include "keyword.h"

#define __RELOAD_WATCHDOG() {               \
    if (watchDog.reloadCounter != NULL) {   \
        watchDog.reloadCounter(&watchDog);  \
    }                                       \
}

typedef struct WatchDog {
    uint8_t _prescaler;
    uint16_t _reload;
    
    void (*_init)(struct WatchDog * this);
    void (*reloadCounter)(struct WatchDog * this);
} WatchDog;

extern PUBLIC STATIC WatchDog watchDog;
PUBLIC void staticWatchDog(void);

#endif // __WATHCDOG_H__
