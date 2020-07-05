#include "selectorgroup.h"

#define SELGRP_MESSAGES_COUNT   16
#define SELGRP_READ_INTERVAL    10

// Private method(s)
PRIVATE void scanSelectorGroup(SelectorGroup * pThis);
PRIVATE void enableSelectorGroupScan(SelectorGroup * pThis);
PRIVATE void disableSelectorGroupScan(SelectorGroup * pThis);

PUBLIC SelectorGroup newSelectorGroup(GPIOPin scanPin, GPIOPin addressPins)
{
    SelectorGroup group = {
        .subject = {
            .registerObserver 
                = (register_observer_fp)registerObserverToSelectorGroup,
            .removeObserver
                = (remove_observer_fp)removeObserverFromSelectorGroup,
            .notifyObservers
                = (notify_observers_fp)notifySelectorGroupObservers
        },

        ._scanPin       = scanPin,
        ._addressPins   = addressPins,

        ._scanThread    = NULL,
        ._messagesQueue = NULL,

        ._selectors     = NULL,
        ._observers     = NULL
    };
    
    setupGPIOPin(&scanPin, INPUT_PULLUP);
    setupGPIOPin(&addressPins, OUTPUT);

    return group;
}

PUBLIC void addSelectorToGroup(SelectorGroup * pThis, DataSelector * sel)
{
    sel->next = pThis->_selectors;
    pThis->_selectors = sel;
}

PUBLIC SelectorMessage getSelectorGroupMessage(SelectorGroup * pThis)
{
    SelectorMessage message = {
        .address    = 0x00, 
        .state      = HIGH
    };

    peekMessageQueue(&pThis->_messagesQueue, &message);
    return message;
}

PUBLIC bool isSelectorGroupScanEnabled(SelectorGroup * pThis)
{
    return ((pThis->_scanThread != NULL) && 
        (osThreadGetState(pThis->_scanThread) != osThreadDeleted));
}

PUBLIC void setSelectorGroupScanEnabled(SelectorGroup * pThis, bool enabled)
{
    if (enabled)
    {
        enableSelectorGroupScan(pThis);
    }
    else
    {
        disableSelectorGroupScan(pThis);
    }
}

PRIVATE void scanSelectorGroup(SelectorGroup * pThis)
{
    SelectorMessage message = {
        .address    = 0x00,
        .state      = HIGH
    };

    for (DataSelector * sel = pThis->_selectors; sel != NULL; sel = sel->next)
    {
        for (uint8_t addr = sel->_startAddress; addr <= sel->_endAddress; addr++)
        {
            writeGPIOPinValue(&pThis->_addressPins, addr);
            
            osDelay(SELGRP_READ_INTERVAL);
            
            message.address = addr;
            message.state = readGPIOPin(&pThis->_scanPin);
            enMessageQueue(&pThis->_messagesQueue, &message);
        }
    }

    while (getMessageQueueLength(&pThis->_messagesQueue) > 0)
    {
        notifySelectorGroupObservers(pThis);
        deMessageQueue(&pThis->_messagesQueue, &message);
    }
}

PRIVATE void enableSelectorGroupScan(SelectorGroup * pThis)
{
    // if scan is ebabled, there is no need to start again
    if (isSelectorGroupScanEnabled(pThis))
    {
        return;
    }
    
    if (pThis->_messagesQueue == NULL)
    {
        pThis->_messagesQueue 
            = newMessageQueue(sizeof(SelectorMessage), SELGRP_MESSAGES_COUNT);
    #ifdef DEBUG
        vQueueAddToRegistry(pThis->_messagesQueue, "SELGRP Queue");
    #endif // DEBUG
    }

    osThreadDef(scanThread, vScanSelectorGroupThread, osPriorityNormal, 0, 128);
    pThis->_scanThread = osThreadCreate(osThread(scanThread), pThis);
}

PRIVATE void disableSelectorGroupScan(SelectorGroup * pThis)
{
    // if scan is disabled, there is no need to stop again
    if (isSelectorGroupScanEnabled(pThis))
    {
        osThreadTerminate(pThis->_scanThread);
    }
}

PUBLIC VIRTUAL void registerObserverToSelectorGroup(SelectorGroup * pThis, 
    struct IObserver * observer)
{
    ChainedObserver * chained = (ChainedObserver *)observer;

    chained->next = pThis->_observers;
    pThis->_observers = chained;
}

PUBLIC VIRTUAL void removeObserverFromSelectorGroup(SelectorGroup * pThis, 
    struct IObserver * observer)
{
    ChainedObserver * o = NULL;
    ChainedObserver * chained = (ChainedObserver *)observer;

    // if observer located in head
    if (pThis->_observers == chained)
    {
        pThis->_observers = chained->next;
        return;
    }

    // if observer not located in head
    for (o = pThis->_observers; o != NULL && o->next != chained; o = o->next)
    {
        // do nothing here
    }

    if (o != NULL)
    {
        o->next = chained->next;
    }
}

PUBLIC VIRTUAL void notifySelectorGroupObservers(SelectorGroup * pThis)
{
    struct ISubject * subject = (struct ISubject *)pThis;

    for (ChainedObserver * o = pThis->_observers; o != NULL; o = o->next)
    {
        struct IObserver * observer = (struct IObserver *)o;
        observer->update(observer, subject);
    }
}

PUBLIC STATIC void vScanSelectorGroupThread(void const * argument)
{
    SelectorGroup * group = (SelectorGroup *)argument;

    LOOP
    {
        scanSelectorGroup(group);
        osDelay(100);
    }
}
