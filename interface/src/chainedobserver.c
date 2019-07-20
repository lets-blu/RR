#include "chainedobserver.h"

PUBLIC ChainedObserver newChainedObserver(void)
{
    ChainedObserver observer = 
    {
        .observer   = 
        {
            .update = (update_observer_fp)defaultUpdateChainedObserver
        }, 

        .next       = NULL
    };

    return observer;
}

PUBLIC VIRTUAL void defaultUpdateChainedObserver(ChainedObserver * pThis, 
    struct ISubject subject)
{
    // do nothing here
}
