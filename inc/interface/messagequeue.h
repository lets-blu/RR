#ifndef __MESSAGEQUEUE_H__
#define __MESSAGEQUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stddef.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#ifdef UNIT_TEST
#include "cmsis_os.h"
#else
#include "FreeRTOS.h"
#include "queue.h"
#endif // UNIT_TEST

#include "keywords.h"

typedef struct {
    void * _base;

    uint8_t _itemCount;
    uint8_t _itemSize;

    uint8_t _headOffset;
    uint8_t _tailOffset;
} MessageQueue;

// Constructor(s)
PUBLIC MessageQueue newMessageQueue(uint8_t itemCount, uint8_t itemSize);

// Public method(s)
PUBLIC void enMessageQueue(MessageQueue * pThis, void * item);
PUBLIC void deMessageQueue(MessageQueue * pThis, void * item);
PUBLIC void peekMessageQueue(MessageQueue * pThis, void * item);

PUBLIC uint8_t getMessageQueueCount(MessageQueue * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __MESSAGEQUEUE_H__
