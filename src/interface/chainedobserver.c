#include "chainedobserver.h"

PUBLIC ChainedObserver newChainedObserver(void)
{
    ChainedObserver observer = {
        .observer = {
            .update = (update_observer_fp)defaultUpdateChainedObserver
        }, 

        ._next = NULL
    };

    return observer;
}

PUBLIC VIRTUAL void defaultUpdateChainedObserver(ChainedObserver * pThis,
    struct ISubject * subject)
{
    (void)pThis;
    (void)subject;
}
