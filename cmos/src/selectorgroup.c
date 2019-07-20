#include "selectorgroup.h"

#define __SELGRP_MESSAGES_COUNT ((uint8_t)32)
#define __SELGRP_READ_INTERVAL  ((uint8_t)10)

// private method(s)
PRIVATE void scanSelectorGroup(SelectorGroup * pThis);
PRIVATE void enableSelectorGroupScan(SelectorGroup * pThis);
PRIVATE void disableSelectorGroupScan(SelectorGroup * pThis);
PRIVATE bool isSelectorGroupScanEnabled(SelectorGroup * pThis);

PRIVATE STATIC void iScanSelectorGroupTask(void const * argument);

PUBLIC SelectorGroup newSelectorGroup(GPIOPin scanPin, GPIOPin addressPins)
{
    SelectorGroup group = 
    {
        .subject        =
        {
            .registerObserver 
                = (register_observer_fp)registerObserverToSelectorGroup, 
            .removeObserver
                = (remove_observer_fp)removeObserverFromSelectorGroup, 
            .notifyObservers
                = (notify_observers_fp)notifySelectorGroupObservers
        }, 

        ._selectors     = NULL, 

        ._scanPin       = scanPin, 
        ._addressPins   = addressPins, 

        ._scanThread    = NULL, 
        ._messages      = NULL,

        ._observers     = NULL
    };

    osMessageQDef(messages, __SELGRP_MESSAGES_COUNT, SelectorMessage);
    group._messages = osMessageCreate(osMessageQ(messages), NULL);
#ifdef DEBUG
    vQueueAddToRegistry(group._messages, "selector group messages queue");
#endif // DEBUG

    setupGPIOPin(&scanPin, INPUT);
    setupGPIOPin(&addressPins, OUTPUT);

    return group;
}

PUBLIC void addSelectorToGroup(SelectorGroup * pThis, DataSelector * selector)
{
    selector->next = pThis->_selectors;
    pThis->_selectors = selector;
}

PUBLIC void setSelectorGroupScan(SelectorGroup *pThis, FunctionalState state)
{
    if (state != DISABLE)
    {
        enableSelectorGroupScan(pThis);
    }
    else
    {
        disableSelectorGroupScan(pThis);
    }
}

PUBLIC SelectorMessage getSelectorGroupMessage(SelectorGroup * pThis)
{
    SelectorMessage message = 
    {
        .address    = 0x00, 
        .state      = HIGH
    };

    xQueuePeek(pThis->_messages, &message, 0);

    return message;
}

PRIVATE void scanSelectorGroup(SelectorGroup * pThis)
{
    SelectorMessage message;

    GPIO_TypeDef * addressPort = getGPIOPinPort(&pThis->_addressPins);
    uint16_t addressMask = getGPIOPinPin(&pThis->_addressPins);
    uint8_t addressOffset = getGPIOPinPinOffset(&pThis->_addressPins);

    for (DataSelector * sel = pThis->_selectors; sel != NULL; sel = sel->next)
    {
        for (uint8_t addr = sel->_startAddress; addr < sel->_endAddress; addr++)
        {
            addressPort->ODR &= ~addressMask;
            addressPort->ODR |= (addr << addressOffset);

            osDelay(__SELGRP_READ_INTERVAL);

            // pack scan result as addr message, and send it to messages queue
            message.address = addr;
            message.state = readGPIOPin(&pThis->_scanPin);

            // considering CMSIS API can not support message type, 
            // use FreeRTOS origin API here
            xQueueSend(pThis->_messages, &message, 0);
        }
    }

    while (uxQueueMessagesWaiting(pThis->_messages) > 0)
    {
        notifySelectorGroupObservers(pThis);
        xQueueReceive(pThis->_messages, &message, 0);
    }
}

PRIVATE void enableSelectorGroupScan(SelectorGroup * pThis)
{
    // if scan is ebabled, there is no need to start again
    if (isSelectorGroupScanEnabled(pThis))
    {
        return;
    }

    osThreadDef(scanTask, iScanSelectorGroupTask, osPriorityNormal, 0, 128);
    osThreadCreate(osThread(scanTask), pThis);
}

PRIVATE void disableSelectorGroupScan(SelectorGroup * pThis)
{
    // if scan is disabled, there is no need to stop again
    if (!isSelectorGroupScanEnabled(pThis))
    {
        return;
    }

    osThreadTerminate(pThis->_scanThread);
}

PRIVATE bool isSelectorGroupScanEnabled(SelectorGroup * pThis)
{
    return ((pThis->_scanThread != NULL)
        && (osThreadGetState(pThis->_scanThread) != osThreadDeleted));
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
    ChainedObserver * chained = (ChainedObserver *)observer;

    // if observer located in head
    if (pThis->_observers == chained)
    {
        pThis->_observers = chained->next;
    }

    // if observer not located in head
    ChainedObserver * p = NULL;

    for (p = pThis->_observers; p != NULL && p->next != chained; p = p->next)
    {
        // do nothing here
    }

    if (p != NULL)
    {
        p->next = chained->next;
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

PRIVATE STATIC void iScanSelectorGroupTask(void const * argument)
{
    SelectorGroup * group = (SelectorGroup *)argument;

    for (;;)
    {
        scanSelectorGroup(group);
        osDelay(100);
    }
}
