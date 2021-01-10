#include "gtest/gtest.h"
#include "chainedobserver.h"

class ChainedObserverTest : public ::testing::Test
{
protected:
    ChainedObserver observer;

    void SetUp()
    {
        observer = newChainedObserver();
    }

    void TearDown()
    {
        // do nothing here
    }
};

int defaultUpdateChainedObserverCallCount;
extern "C" void MOCKABLE(defaultUpdateChainedObserver)(
    ChainedObserver * pThis, ISubject * subject);

TEST_F(ChainedObserverTest, defaultUpdateChainedObserver)
{
    defaultUpdateChainedObserverCallCount = 0;

    observer.observer.update(&observer.observer, NULL);
    EXPECT_EQ(1, defaultUpdateChainedObserverCallCount);
}

PUBLIC VIRTUAL void defaultUpdateChainedObserver(
    ChainedObserver * pThis, ISubject * subject)
{
    MOCKABLE(defaultUpdateChainedObserver)(pThis, subject);
    defaultUpdateChainedObserverCallCount++;
}
