#ifndef __SELECTORGROUP_H__
#define __SELECTORGROUP_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

#include "chainedobserver.h"
#include "dataselector.h"
#include "gpiopin.h"
#include "keywords.h"
#include "messagequeue.h"
#include "observer.h"
#include "selectormessage.h"
#include "ut.h"

#ifdef UNIT_TEST
#include "ut_cmsis_os.h"
#include "ut_hal.h"
#include "ut_hal_gpio.h"
#else
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#endif // UNIT_TEST

typedef struct {
    struct ISubject subject; // implementation, must be the first

    GPIOPin _scanPin;
    GPIOPin _addressPins;

    osThreadId _scanThread;
    MessageQueue _messagesQueue;

    DataSelector * _selectors;
    ChainedObserver * _observers;
} SelectorGroup;

// Constructor
PUBLIC SelectorGroup newSelectorGroup(GPIOPin scanPin, GPIOPin addressPins);

// Public method(s)
PUBLIC void addSelectorToGroup(SelectorGroup * pThis, DataSelector * sel);
PUBLIC SelectorMessage getSelectorGroupMessage(SelectorGroup * pThis);

PUBLIC bool isSelectorGroupScanEnabled(SelectorGroup * pThis);
PUBLIC void setSelectorGroupScanEnabled(SelectorGroup * pThis, bool enabled);
PUBLIC STATIC void vScanSelectorGroupThread(void const * argument);

PUBLIC VIRTUAL void registerObserverToSelectorGroup(SelectorGroup * pThis, 
    struct IObserver * observer);
PUBLIC VIRTUAL void removeObserverFromSelectorGroup(SelectorGroup * pThis, 
    struct IObserver * observer);
PUBLIC VIRTUAL void notifySelectorGroupObservers(SelectorGroup * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SELECTORGROUP_H__
