#ifndef __CHAINEDOBSERVER_H__
#define __CHAINEDOBSERVER_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdlib.h"

#include "keywords.h"
#include "observer.h"

typedef struct ChainedObserver
{
    struct IObserver observer;
    struct ChainedObserver * next;
} ChainedObserver;

// constructor
PUBLIC ChainedObserver newChainedObserver(void);

// public method(s)
PUBLIC VIRTUAL void updateChainedObserver(ChainedObserver * pThis, 
    struct ISubject subject);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __CHAINEDOBSERVER_H__
