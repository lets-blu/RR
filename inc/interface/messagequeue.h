#ifndef __MESSAGEQUEUE_H__
#define __MESSAGEQUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#include "cmsis_os.h"

#include "keywords.h"

typedef struct {
    void * _base;

    uint8_t _queueLength;
    uint8_t _itemSize;

    uint8_t _headOffset;
    uint8_t _tailOffset;
} MessageQueue;

typedef void MessageQueueItem;

// Constructor
PUBLIC MessageQueue newMessageQueue(uint8_t queueLength, uint8_t itemSize);

// Public method(s)
PUBLIC void enMessageQueue(MessageQueue * pThis, MessageQueueItem * item);
PUBLIC void deMessageQueue(MessageQueue * pThis, MessageQueueItem * item);
PUBLIC void peekMessageQueue(MessageQueue * pThis, MessageQueueItem * item);

PUBLIC uint8_t getMessageQueueItemCount(MessageQueue * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __MESSAGEQUEUE_H__
