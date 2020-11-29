#include "messagequeue.h"

// Private method(s)
PRIVATE STATIC void * createMessageQueueBase(uint8_t itemCount, uint8_t itemSize);

#ifdef UNIT_TEST
PUBLIC MessageQueue newMessageQueue(uint8_t itemCount, uint8_t itemSize)
{
    MessageQueue queue = {
        ._base          = createMessageQueueBase(itemCount, itemSize),

        ._itemCount     = itemCount,
        ._itemSize      = itemSize,

        ._headOffset    = 0,
        ._tailOffset    = 0
    };

    return queue;
}

PUBLIC void enMessageQueue(MessageQueue * pThis, void * item)
{
    if (pThis->_base == NULL)
    {
        return;
    }

    if (getMessageQueueCount(pThis) < pThis->_itemCount)
    {
        memcpy(pThis->_base + pThis->_tailOffset, item, pThis->_itemSize);
        pThis->_tailOffset += pThis->_itemSize;
    }
}

PUBLIC void deMessageQueue(MessageQueue * pThis, void * item)
{
    if (pThis->_base == NULL)
    {
        return;
    }

    if (getMessageQueueCount(pThis) > 0)
    {
        memcpy(item, pThis->_base + pThis->_headOffset, pThis->_itemSize);
        pThis->_headOffset += pThis->_itemSize;

        // reset head and tail offset
        if (pThis->_headOffset == pThis->_tailOffset)
        {
            pThis->_headOffset = 0;
            pThis->_tailOffset = 0;
        }
    }
}
PUBLIC void peekMessageQueue(MessageQueue * pThis, void * item)
{
    if (pThis->_base == NULL)
    {
        return;
    }

    if (getMessageQueueCount(pThis) > 0)
    {
        memcpy(item, pThis->_base + pThis->_headOffset, pThis->_itemSize);
    }
}

PUBLIC uint8_t getMessageQueueCount(MessageQueue * pThis)
{
    return ((pThis->_tailOffset - pThis->_headOffset) / pThis->_itemSize);
}

PRIVATE STATIC void * createMessageQueueBase(uint8_t itemCount, uint8_t itemSize)
{
    return malloc(itemCount * itemSize);
}
#else
PUBLIC void enMessageQueue(MessageQueue * pThis, void * item)
{
    xQueueSend(pThis->_base, item, 0);
}

PUBLIC void deMessageQueue(MessageQueue * pThis, void * item)
{
    xQueueReceive(pThis->_base, item, 0);
}

PUBLIC void peekMessageQueue(MessageQueue * pThis, void * item)
{
    xQueuePeek(pThis->_base, item, 0);
}

PUBLIC uint8_t getMessageQueueCount(MessageQueue * pThis)
{
    return uxQueueMessagesWaiting(pThis->_base);
}

PRIVATE STATIC void * createMessageQueueBase(uint8_t itemCount, uint8_t itemSize)
{
    return xQueueCreate(itemCount, itemSize);
}
#endif // UNIT_TEST
