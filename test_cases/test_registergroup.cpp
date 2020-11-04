#include "stdint.h"
#include "gtest/gtest.h"

#include "shiftregister.h"
#include "registergroup.h"

#define TEST_BIT2MASK(bit)  (1 << (bit))
#define TEST_REGISTER(bit)  shiftRegister[(bit) / 8]

class RegisterGroupTest : public ::testing::Test
{
protected:
    const uint8_t BIT = 7;
    static const int REGISTER_COUNT = 2;

    GPIO_TypeDef * const PORT = GPIOA;
    const uint16_t OE_PIN = GPIO_PIN_4;
    const uint16_t SER_PIN = GPIO_PIN_7;
    const uint16_t SCK_PIN = GPIO_PIN_5;
    const uint16_t RCK_PIN = GPIO_PIN_3;

    RegisterGroup registerGroup;
    ShiftRegister shiftRegister[REGISTER_COUNT];

    void SetUp()
    {
        RegisterGroupPins pins = {
            .oePin  = newGPIOPin(PORT, OE_PIN),
            .serPin = newGPIOPin(PORT, SER_PIN),
            .sckPin = newGPIOPin(PORT, SCK_PIN),
            .rckPin = newGPIOPin(PORT, RCK_PIN)
        };

        // construct Register Group
        registerGroup = newRegisterGroup(pins);

        // construct Shift Registers
        for (int i = 0; i < REGISTER_COUNT; i++)
        {
            shiftRegister[i] = newShiftRegister();
            addRegisterGroupRegister(&registerGroup, &shiftRegister[i]);
        }
    }

    void TearDown()
    {
        // do noting here
    }
};

TEST_F(RegisterGroupTest, addRegisterGroupRegister)
{
    EXPECT_EQ(&shiftRegister[REGISTER_COUNT - 1], registerGroup._registers);

    for (int i = 0; i < REGISTER_COUNT - 1; i++)
    {
        EXPECT_EQ(&shiftRegister[i], shiftRegister[i + 1]._next);
    }
}

TEST_F(RegisterGroupTest, setRegisterGroupBit)
{
    setRegisterGroupBit(&registerGroup, BIT);
    EXPECT_EQ(TEST_BIT2MASK(BIT % 8), TEST_REGISTER(BIT)._data & TEST_BIT2MASK(BIT % 8));
}

TEST_F(RegisterGroupTest, resetRegisterGroupBit)
{
    resetRegisterGroupBit(&registerGroup, BIT);
    EXPECT_EQ(0, TEST_REGISTER(BIT)._data & TEST_BIT2MASK(BIT % 8));
}

TEST_F(RegisterGroupTest, setRegisterGroupOutputEnabled)
{
    setRegisterGroupOutputEnabled(&registerGroup, true);
    EXPECT_EQ(0, PORT->ODR & OE_PIN);

    setRegisterGroupOutputEnabled(&registerGroup, false);
    EXPECT_EQ(OE_PIN, PORT->ODR & OE_PIN);
}

TEST_F(RegisterGroupTest, outputRegisterGroup)
{
    
}
