#include "gtest/gtest.h"
#include "chainedobserver.h"

class ChainedObserverTest : public ::testing::Test
{
public:
    static int defaultUpdateChainedObserverCallCount;

protected:
    ChainedObserver observer;

    void SetUp()
    {
        observer = newChainedObserver();
        observer.observer.update = (update_observer_fp)defaultUpdateChainedObserver;
    }

    void TearDown()
    {
        // do nothing here
    }
};

int ChainedObserverTest::defaultUpdateChainedObserverCallCount;

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
    ChainedObserverTest::defaultUpdateChainedObserverCallCount++;
}
