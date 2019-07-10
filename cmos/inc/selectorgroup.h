#ifndef __SELECTORGROUP_H__
#define __SELECTORGROUP_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdlib.h"
#include "stdbool.h"

#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

#include "chainedobserver.h"
#include "dataselector.h"
#include "gpiopin.h"
#include "keywords.h"
#include "observer.h"
#include "selectormessage.h"

typedef struct
{
    struct ISubject subject;
    ChainedObserver * _observers;

    DataSelector * _selectors;

    GPIOPin _scanPin;
    GPIOPin _addressPins;

    osThreadId _scanThread;
    osMessageQId _messages;
} SelectorGroup;

// constructor
PUBLIC SelectorGroup newSelectorGroup(GPIOPin scanPin, GPIOPin addressPins);

// public method(s)
PUBLIC void addSelectorToGroup(SelectorGroup * pThis, DataSelector * selector);
PUBLIC void setSelectorGroupScan(SelectorGroup * pThis, FunctionalState state);

PUBLIC VIRTUAL void registerObserverToSelectorGroup(SelectorGroup * pThis, 
    struct IObserver * observer);
PUBLIC VIRTUAL void removeObserverFromSelectorGroup(SelectorGroup * pThis, 
    struct IObserver * observer);
PUBLIC VIRTUAL void notifySelectorGroupObservers(SelectorGroup * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SELECTORGROUP_H__
