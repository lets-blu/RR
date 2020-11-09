#include "stddef.h"
#include "stdint.h"
#include "gtest/gtest.h"

#include "shiftregister.h"
#include "registergroup.h"

#define TEST_SLL(data)      ((data) <<= 1)
#define TEST_MSB(data)      ((data) & 0x80)

#define TEST_BIT2MASK(bit)  (0x01U << (bit))

class RegisterGroupTest : public ::testing::Test
{
protected:
    const uint8_t BIT = 7;
    static const int REGISTERS_COUNT = 2;

    GPIO_TypeDef * const PORT = GPIOA;
    const uint16_t OE_PIN = GPIO_PIN_4;
    const uint16_t SER_PIN = GPIO_PIN_7;
    const uint16_t SCK_PIN = GPIO_PIN_5;
    const uint16_t RCK_PIN = GPIO_PIN_3;

    RegisterGroup registerGroup;
    ShiftRegister shiftRegisters[REGISTERS_COUNT];

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
        for (int i = 0; i < REGISTERS_COUNT; i++)
        {
            shiftRegisters[i] = newShiftRegister();
            addRegisterGroupRegister(&registerGroup, &shiftRegisters[i]);
        }
    }

    void TearDown()
    {
        // do noting here
    }

    uint32_t getOutputData(uint8_t bit)
    {
        if (bit * 3 < GPIO_ARRAY_SIZE)
        {
            return PORT->ODRArray[bit * 3];
        }

        return 0;
    }

    ShiftRegister * getShiftRegister(uint8_t bit)
    {
        if (bit / 8 < REGISTERS_COUNT)
        {
            return &shiftRegisters[bit / 8];
        }

        return NULL;
    }
};

TEST_F(RegisterGroupTest, addRegisterGroupRegister)
{
    EXPECT_EQ(&shiftRegisters[REGISTERS_COUNT - 1], registerGroup._registers);

    for (int i = 0; i < REGISTERS_COUNT - 1; i++)
    {
        EXPECT_EQ(&shiftRegisters[i], shiftRegisters[i + 1]._next);
    }
}

TEST_F(RegisterGroupTest, setRegisterGroupBit)
{
    setRegisterGroupBit(&registerGroup, BIT);
    ShiftRegister * shiftRegister = getShiftRegister(BIT);

    if (shiftRegister != NULL)
    {
        EXPECT_EQ(TEST_BIT2MASK(BIT % 8), shiftRegister->_data & TEST_BIT2MASK(BIT % 8));
    }
}

TEST_F(RegisterGroupTest, resetRegisterGroupBit)
{
    resetRegisterGroupBit(&registerGroup, BIT);
    ShiftRegister * shiftRegister = getShiftRegister(BIT);

    if (shiftRegister != NULL)
    {
        EXPECT_EQ(0x00U, shiftRegister->_data & TEST_BIT2MASK(BIT % 8));
    }
}

TEST_F(RegisterGroupTest, setRegisterGroupOutputEnabled)
{
    GPIO_RESET(PORT);

    setRegisterGroupOutputEnabled(&registerGroup, true);
    EXPECT_EQ(0x00U, PORT->ODRArray[0] & OE_PIN);

    setRegisterGroupOutputEnabled(&registerGroup, false);
    EXPECT_EQ(OE_PIN, PORT->ODRArray[1] & OE_PIN);
}

TEST_F(RegisterGroupTest, outputRegisterGroup)
{
    setRegisterGroupBit(&registerGroup, BIT);
    setRegisterGroupOutputEnabled(&registerGroup, true);

    GPIO_RESET(PORT);
    outputRegisterGroup(&registerGroup);

    for (int index = 0; index < REGISTERS_COUNT; index++)
    {
        uint8_t data = shiftRegisters[REGISTERS_COUNT - index - 1]._data;

        for (int bit = 0; bit < 8; bit++)
        {
            if (TEST_MSB(data) == 0x00)
            {
                EXPECT_EQ(0x00U, getOutputData(index * 8 + bit) & SER_PIN);
            }
            else
            {
                EXPECT_EQ(SER_PIN, getOutputData(index * 8 + bit) & SER_PIN);
            }

            TEST_SLL(data);
        }
    }
}
