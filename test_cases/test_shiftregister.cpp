#include "shiftregister.h"
#include "gtest/gtest.h"

#define TEST_BIT2MASK(bit)  (1 << (bit))

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
    EXPECT_EQ(TEST_BIT2MASK(BIT), shiftRegister._data & TEST_BIT2MASK(BIT));
}

TEST_F(ShiftRegisterTest, resetShiftRegisterBit)
{
    resetShiftRegisterBit(&shiftRegister, BIT);
    EXPECT_EQ(0x00U, shiftRegister._data & TEST_BIT2MASK(BIT));
}
