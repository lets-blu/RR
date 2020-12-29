#ifndef __CHAINEDOBSERVER_H__
#define __CHAINEDOBSERVER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stddef.h"

#include "keywords.h"
#include "observer.h"
#include "testing.h"

typedef struct ChainedObserver {
    IObserver observer;
    struct ChainedObserver * _next;
} ChainedObserver;

// Constructor
PUBLIC ChainedObserver newChainedObserver(void);

// Public method(s)
PUBLIC VIRTUAL void defaultUpdateChainedObserver(
    ChainedObserver * pThis, ISubject * subject);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __CHAINEDOBSERVER_H__
