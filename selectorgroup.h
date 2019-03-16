#ifndef __SELECTORGROUP_H__
#define __SELECTORGROUP_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "assert.h"
#include "stdlib.h"

#include "dataselector.h"
#include "gpiopin.h"
#include "keywords.h"
#include "observer.h"
#include "selectormessage.h"

typedef struct SelectorGroup {
    struct ISubject subject;

    DataSelector * _head;

    GPIOPin _scanPin;
    uint16_t _addressMask;
    GPIO_TypeDef * _addressPort;

    xQueueHandle _xMessagesQueue;
    struct IObserver ** _observers;
    uint8_t _totalOfObservers, _numOfObservers;
} SelectorGroup;

// (de)constructor(s)
PUBLIC SelectorGroup newSelectorGroup(uint8_t totalOfObservers);
PUBLIC void deleteSelectorGroup(SelectorGroup * pThis);

// public method(s)
PUBLIC void setSelectorGroupScanPin(SelectorGroup * pThis, GPIOPin scanPin);
PUBLIC void setSelectorGroupAddressPins(SelectorGroup * pThis,
    GPIOPin addressLowerPin, uint8_t addressWidth);
PUBLIC void addSelectorToGroup(SelectorGroup * pThis, 
    DataSelector * selector);
PUBLIC void scanSelectorGroup(SelectorGroup * pThis);

PUBLIC VIRTUAL void registerObserverToSelectorGroup(SelectorGroup * pThis, 
    struct IObserver * observer);
PUBLIC VIRTUAL void removeObserverFromSelectorGroup(SelectorGroup * pThis, 
    struct IObserver * observer);
PUBLIC VIRTUAL void notifyAllSelectorGroupObservers(SelectorGroup * pThis);

PUBLIC STATIC void vScanSelectorGroupTask(void * pSelectorGroup);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SELECTORGROUP_H__
