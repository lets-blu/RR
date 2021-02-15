#ifndef __SELECTORGROUP_H__
#define __SELECTORGROUP_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

#include "cmsis_os.h"

#include "chainedobserver.h"
#include "dataselector.h"
#include "gpiopin.h"
#include "keywords.h"
#include "messagequeue.h"
#include "observer.h"
#include "selectormessage.h"

typedef struct {
    ISubject subject;

    GPIOPin _scanPin;
    GPIOPin _addressPins;

    osThreadId _scanThread;
    MessageQueue _messagesQueue;

    DataSelector * _selectors;
    ChainedObserver * _observers;
} SelectorGroup;

// Constructor
PUBLIC SelectorGroup newSelectorGroup(GPIOPin scanPin, GPIOPin addressPins);
PUBLIC void deleteSelectorGroup(SelectorGroup * pThis);

// Public method(s)
PUBLIC void addSelectorGroupSelector(SelectorGroup * pThis, DataSelector * sel);
PUBLIC SelectorMessage peekSelectorGroupMessage(SelectorGroup * pThis);

PUBLIC bool isSelectorGroupScanEnabled(SelectorGroup * pThis);
PUBLIC void setSelectorGroupScanEnabled(SelectorGroup * pThis, bool enabled);

PUBLIC VIRTUAL void attachSelectorGroupObserver(
    SelectorGroup * pThis, IObserver * observer);
PUBLIC VIRTUAL void detachSelectorGroupObserver(
    SelectorGroup * pThis, IObserver * observer);
PUBLIC VIRTUAL void notifySelectorGroupObservers(SelectorGroup * pThis);

PUBLIC STATIC void vScanSelectorGroupThread(void const * argument);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SELECTORGROUP_H__
