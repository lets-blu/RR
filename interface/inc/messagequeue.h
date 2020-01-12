#ifndef __MESSAGEQUEUE_H__
#define __MESSAGEQUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdbool.h"
#include "stdint.h"

#include "keywords.h"

#ifdef UNIT_TEST
#include "stdlib.h"
#include "string.h"
#else
#include "FreeRTOS.h"
#include "queue.h"
#endif // UNIT_TEST

typedef void * MessageQueue;

// Constructor
PUBLIC MessageQueue newMessageQueue(uint8_t itemSize, uint8_t itemCount);

// Public method(s)
PUBLIC bool enMessageQueue(MessageQueue * pThis, void * item);
PUBLIC bool deMessageQueue(MessageQueue * pThis, void * item);
PUBLIC bool peekMessageQueue(MessageQueue * pThis, void * item);

PUBLIC uint8_t getMessageQueueLength(MessageQueue * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __MESSAGEQUEUE_H__
