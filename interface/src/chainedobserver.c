#include "chainedobserver.h"

PUBLIC ChainedObserver newChainedObserver(void)
{
    ChainedObserver observer = 
    {
        .observer   = 
        {
            .update = (update_observer_fp)updateChainedObserver
        }, 

        .next       = NULL
    };

    return observer;
}

PUBLIC VIRTUAL void updateChainedObserver(ChainedObserver * pThis, 
    struct ISubject subject)
{
    // do nothing here
}
