#include "selectorgroup.h"

#define SELGRP_READ_INTERVAL    10U
#define SELGRP_SCAN_INTERVAL    100U

// Private method(s)
PRIVATE void scanSelectorGroup(SelectorGroup * pThis);

PRIVATE void enableSelectorGroupScan(SelectorGroup * pThis);
PRIVATE void disableSelectorGroupScan(SelectorGroup * pThis);

PRIVATE uint8_t getSelectorGroupAddressCount(SelectorGroup * pThis);

PUBLIC SelectorGroup newSelectorGroup(GPIOPin scanPin, GPIOPin addressPins)
{
    SelectorGroup group = {
        .subject = {
            .attach = (attach_observer_fp)attachSelectorGroupObserver,
            .detach = (detach_observer_fp)detachSelectorGroupObserver,
            .notify = (notify_observer_fp)notifySelectorGroupObservers
        },

        ._scanPin       = scanPin,
        ._addressPins   = addressPins,

        ._scanThread    = NULL,
        ._messagesQueue = {0},

        ._selectors     = NULL,
        ._observers     = NULL
    };
    
    setupGPIOPin(&scanPin, INPUT_PULLUP);
    setupGPIOPin(&addressPins, OUTPUT);

    return group;
}

PUBLIC void deleteSelectorGroup(SelectorGroup * pThis)
{
    if (pThis->_messagesQueue._base != NULL)
    {
        deleteMessageQueue(&pThis->_messagesQueue);
    }
}

PUBLIC void addSelectorGroupSelector(SelectorGroup * pThis, DataSelector * sel)
{
    sel->_next = pThis->_selectors;
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
    return ((pThis->_scanThread != NULL)
        && (osThreadGetState(pThis->_scanThread) != osThreadDeleted));
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

    for (DataSelector * sel = pThis->_selectors; sel != NULL; sel = sel->_next)
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

    while (getMessageQueueItemCount(&pThis->_messagesQueue) > 0)
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

    if (pThis->_messagesQueue._base == NULL)
    {
        uint8_t queueLength = getSelectorGroupAddressCount(pThis);
        pThis->_messagesQueue = newMessageQueue(queueLength, sizeof(SelectorMessage));
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

PRIVATE uint8_t getSelectorGroupAddressCount(SelectorGroup * pThis)
{
    uint8_t count = 0;

    for (DataSelector * sel = pThis->_selectors; sel != NULL; sel = sel->_next)
    {
        for (uint8_t addr = sel->_startAddress; addr <= sel->_endAddress; addr++)
        {
            count++;
        }
    }

    return count;
}

PUBLIC VIRTUAL void attachSelectorGroupObserver(
    SelectorGroup * pThis, IObserver * observer)
{
    ChainedObserver * chained = (ChainedObserver *)observer;

    chained->_next = pThis->_observers;
    pThis->_observers = chained;
}

PUBLIC VIRTUAL void detachSelectorGroupObserver(
    SelectorGroup * pThis, IObserver * observer)
{
    ChainedObserver * o = NULL;
    ChainedObserver * chained = (ChainedObserver *)observer;

    // if observer located in head
    if (pThis->_observers == chained)
    {
        pThis->_observers = chained->_next;
        return;
    }

    // if observer not located in head
    for (o = pThis->_observers; o != NULL && o->_next != chained; o = o->_next)
    {
        // do noting here
    }

    if (o != NULL)
    {
        o->_next = chained->_next;
    }
}

PUBLIC VIRTUAL void notifySelectorGroupObservers(SelectorGroup * pThis)
{
    for (ChainedObserver * o = pThis->_observers; o != NULL; o = o->_next)
    {
        o->observer.update(&o->observer, &pThis->subject);
    }
}

PUBLIC STATIC void vScanSelectorGroupThread(void const * argument)
{
    SelectorGroup * group = (SelectorGroup *)argument;

    LOOP
    {
        scanSelectorGroup(group);
        osDelay(SELGRP_SCAN_INTERVAL);
    }
}
