#include "gtest/gtest.h"
#include "dataselector.h"

class DataSelectorTest : public ::testing::Test
{
protected:
    const uint8_t START_ADDRESS = 0;
    const uint8_t END_ADDRESS = 7;

    DataSelector dataSelector;

    void SetUp()
    {
        dataSelector = newDataSelector(START_ADDRESS, END_ADDRESS);
    }

    void TearDown()
    {
        // do nothing here
    }
};
