#include "selectorgroup.h"

#define __READ_INTERVAL ((uint8_t)1)

// private method(s)
PRIVATE void scanSelectorsInGroup(SelectorGroup * pThis, DataSelector * selector);
PRIVATE uint8_t getSelectorGroupAddressOffset(SelectorGroup * pThis);

PUBLIC SelectorGroup newSelectorGroup(uint8_t totalOfObservers)
{
    SelectorGroup group = {
        .subject.registerObserver
            = (register_observer_fp)registerObserverToSelectorGroup,
        .subject.removeObserver
            = (remove_observer_fp)removeObserverFromSelectorGroup,
        .subject.notifyAllObservers
            = (notify_observers_fp)notifyAllSelectorGroupObservers,

        ._head              = NULL,

        ._addressMask       = 0, 
        ._addressPort       = NULL, 

        ._xMessagesQueue    = NULL,
        ._observers         = NULL,
        ._totalOfObservers  = 0,
        ._numOfObservers    = 0
    };

    uint8_t length = 2 * totalOfObservers * 8;
    group._xMessagesQueue = xQueueCreate(length, sizeof(SelectorMessage));
#ifdef DEBUG
    vQueueAddToRegistry(group._xMessagesQueue, "Selector Group Messages Queue");
#endif // DEBUG

    group._observers = malloc(totalOfObservers * sizeof(struct IObserver *));
    group._totalOfObservers = group._observers == NULL ? 0 : totalOfObservers;

    return group;
}

PUBLIC void deleteSelectorGroup(SelectorGroup * pThis)
{
    free(pThis->_observers);
    pThis->_observers = NULL;
}

PUBLIC void setSelectorGroupScanPin(SelectorGroup * pThis, GPIOPin scanPin)
{
    pThis->_scanPin = scanPin;
    setupGPIOPin(&pThis->_scanPin, INPUT);
}

PUBLIC void setSelectorGroupAddressPins(SelectorGroup * pThis,
    GPIOPin addressLowerPin, uint8_t addressWidth)
{
    GPIOPin addressPin;
    assert_param(addressWidth <= 16);

    pThis->_addressPort = addressLowerPin._port;
    for (uint8_t i = 0; i < addressWidth; ++i) {
        addressPin = newGPIOPin(addressLowerPin._port, addressLowerPin._pin << i);
        setupGPIOPin(&addressPin, OUTPUT);

        pThis->_addressMask |= (addressLowerPin._pin << i);
    }
}

PUBLIC void addSelectorToGroup(SelectorGroup * pThis,
    DataSelector * selector)
{
    selector->next = pThis->_head;
    pThis->_head = selector;
}

PUBLIC void scanSelectorGroup(SelectorGroup * pThis)
{
    DataSelector * selector = pThis->_head;
    for (; selector != NULL; selector = selector->next) {
        scanSelectorsInGroup(pThis, selector);
    }
}

PUBLIC VIRTUAL void registerObserverToSelectorGroup(SelectorGroup * pThis,
    struct IObserver * observer)
{
    if (pThis->_observers != NULL 
        && pThis->_numOfObservers < pThis->_totalOfObservers) {
        pThis->_observers[pThis->_numOfObservers++] = observer;
    }
}

PUBLIC VIRTUAL void removeObserverFromSelectorGroup(SelectorGroup * pThis,
    struct IObserver * observer)
{
    // not supported yet
}

PUBLIC VIRTUAL void notifyAllSelectorGroupObservers(SelectorGroup * pThis)
{
    for (uint8_t i = 0; i < pThis->_numOfObservers; ++i) {
        struct IObserver * observer = pThis->_observers[i];
        observer->update(observer, (struct ISubject *)pThis);
    }
}

PRIVATE uint8_t getSelectorGroupAddressOffset(SelectorGroup * pThis)
{
    uint8_t offset = 0;

    for (uint16_t i = 0x01; (pThis->_addressMask & i) == 0x00; i <<= 1, ++offset) {
        ; // do nothing
    }

    return offset;
}

PRIVATE void scanSelectorsInGroup(SelectorGroup * pThis, DataSelector * selector)
{
    SelectorMessage message;
	uint8_t offset = getSelectorGroupAddressOffset(pThis);

    if (pThis->_addressPort == NULL) {
        return;
    }
	
    uint8_t address = selector->_startAddress;
    for (; address <= selector->_endAddress; ++address) {
        pThis->_addressPort->ODR &= ~(pThis->_addressMask);
        pThis->_addressPort->ODR |= (address << offset);

        vTaskDelay(__READ_INTERVAL);

        message.id = address;
        message.data = readGPIOPin(&pThis->_scanPin);
        xQueueSendToBack(pThis->_xMessagesQueue, &message, 0);
    }
}

PUBLIC STATIC void vScanSelectorGroupTask(void * pSelectorGroup)
{
    SelectorMessage message;
    SelectorGroup * group = (SelectorGroup *)pSelectorGroup;

    for (;;) {
        scanSelectorGroup(group);

        while (uxQueueMessagesWaiting(group->_xMessagesQueue) > 0) {
            notifyAllSelectorGroupObservers(group);
            xQueueReceive(group->_xMessagesQueue, &message, 0);
        }
    }
}
