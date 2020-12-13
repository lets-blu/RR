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

TEST_F(ChainedObserverTest, defaultUpdateChainedObserver)
{
    // do nothing here
}
