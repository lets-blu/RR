#include "messagequeue.h"

// Private method(s)
PRIVATE STATIC void * createMessageQueueBase(uint8_t queueLength, uint8_t itemSize);

PUBLIC MessageQueue newMessageQueue(uint8_t queueLength, uint8_t itemSize)
{
    MessageQueue queue = {
        ._base          = createMessageQueueBase(queueLength, itemSize),

        ._queueLength   = queueLength,
        ._itemSize      = itemSize,

        ._headOffset    = 0,
        ._tailOffset    = 0
    };

    return queue;
}

#ifdef UNIT_TEST
PUBLIC void enMessageQueue(MessageQueue * pThis, MessageQueueItem * item)
{
    if (getMessageQueueItemCount(pThis) < pThis->_queueLength)
    {
        memcpy(pThis->_base + pThis->_tailOffset, item, pThis->_itemSize);
        pThis->_tailOffset += pThis->_itemSize;
    }
}

PUBLIC void deMessageQueue(MessageQueue * pThis, MessageQueueItem * item)
{
    if (getMessageQueueItemCount(pThis) > 0)
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

PUBLIC void peekMessageQueue(MessageQueue * pThis, MessageQueueItem * item)
{
    if (getMessageQueueItemCount(pThis) > 0)
    {
        memcpy(item, pThis->_base + pThis->_headOffset, pThis->_itemSize);
    }
}

PUBLIC uint8_t getMessageQueueItemCount(MessageQueue * pThis)
{
    return ((pThis->_tailOffset - pThis->_headOffset) / pThis->_itemSize);
}

PRIVATE STATIC void * createMessageQueueBase(uint8_t queueLength, uint8_t itemSize)
{
    return malloc(queueLength * itemSize);
}
#else
PUBLIC void enMessageQueue(MessageQueue * pThis, MessageQueueItem * item)
{
    xQueueSend(pThis->_base, item, portMAX_DELAY);
}

PUBLIC void deMessageQueue(MessageQueue * pThis, MessageQueueItem * item)
{
    xQueueReceive(pThis->_base, item, portMAX_DELAY);
}

PUBLIC void peekMessageQueue(MessageQueue * pThis, MessageQueueItem * item)
{
    xQueuePeek(pThis->_base, item, portMAX_DELAY);
}

PUBLIC uint8_t getMessageQueueItemCount(MessageQueue * pThis)
{
    return uxQueueMessagesWaiting(pThis->_base);
}

PRIVATE STATIC void * createMessageQueueBase(uint8_t queueLength, uint8_t itemSize)
{
    return xQueueCreate(queueLength, itemSize);
}
#endif // UNIT_TEST
