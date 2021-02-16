#include "gtest/gtest.h"
#include "watchdog.h"

class WatchdogTest : public ::testing::Test
{
protected:
    Watchdog watchdog;

    const uint16_t TIMEOUT = 1000;

    void SetUp()
    {
        watchdog = newWatchdog(TIMEOUT);
    }

    void TearDown()
    {
        // do nothing here
    }
};

TEST_F(WatchdogTest, enableWatchdog)
{
    HAL_IWDG_InitCallCount = 0;

    enableWatchdog(&watchdog);
    EXPECT_EQ(1, HAL_IWDG_InitCallCount);
}

TEST_F(WatchdogTest, reloadWatchdog)
{
    HAL_IWDG_RefreshCallCount = 0;

    reloadWatchdog(&watchdog);
    EXPECT_EQ(1, HAL_IWDG_RefreshCallCount);
}
