#include "shiftregister.h"
#include "gtest/gtest.h"

class ShiftRegisterTest : public ::testing::Test
{
protected:
    ShiftRegister shiftRegister;
    const uint8_t BIT = 0;

    void SetUp()
    {
        shiftRegister = newShiftRegister();
    }

    void TearDown()
    {
        // do nothing here
    }
};

TEST_F(ShiftRegisterTest, setShiftRegisterBit)
{
    setShiftRegisterBit(&shiftRegister, BIT);
    EXPECT_EQ(1 << BIT, shiftRegister._data & (1 << BIT));
}

TEST_F(ShiftRegisterTest, resetShiftRegisterBit)
{
    resetShiftRegisterBit(&shiftRegister, BIT);
    EXPECT_EQ(0, shiftRegister._data & (1 << BIT));
}
