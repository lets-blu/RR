#include "messagequeue.h"

PUBLIC MessageQueue newMessageQueue(uint8_t itemSize, uint8_t itemCount)
{
    return xQueueCreate(itemCount, itemSize);
}

PUBLIC bool enMessageQueue(MessageQueue * pThis, void * item)
{
    return (xQueueSend(*pThis, item, 0) == pdTRUE);
}

PUBLIC bool deMessageQueue(MessageQueue * pThis, void * item)
{
    return (xQueueReceive(*pThis, item, 0) == pdTRUE);
}

PUBLIC bool peekMessageQueue(MessageQueue * pThis, void * item)
{
    return (xQueuePeek(*pThis, item, 0) == pdTRUE);
}

PUBLIC uint8_t getMessageQueueLength(MessageQueue * pThis)
{
    return uxQueueMessagesWaiting(*pThis);
}
