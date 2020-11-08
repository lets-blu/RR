#include "gpiopin.h"
#include "gtest/gtest.h"

class GPIOPinTest : public ::testing::Test
{
protected:
    GPIOPin pin;
    const uint16_t PIN = GPIO_PIN_13;
    GPIO_TypeDef * const PORT = GPIOC;
    const uint32_t INVALID_MODE = 0xFFFFFFFFU;

    void SetUp()
    {
        pin = newGPIOPin(PORT, PIN);
    }

    void TearDown()
    {
        // do nothing here
    }
};

TEST_F(GPIOPinTest, setupGPIOPin)
{
    HAL_GPIO_InitCallCount = 0;

    setupGPIOPin(&pin, INVALID_MODE);
    EXPECT_EQ(0, HAL_GPIO_InitCallCount);

    setupGPIOPin(&pin, OUTPUT);
    EXPECT_EQ(1, HAL_GPIO_InitCallCount);
}

TEST_F(GPIOPinTest, readGPIOPin)
{
    GPIO_RESET(PORT);

    PORT->IDRArray[0] &= ~PIN;
    EXPECT_EQ(LOW, readGPIOPin(&pin));

    PORT->IDRArray[1] |= PIN;
    EXPECT_EQ(HIGH, readGPIOPin(&pin));
}

TEST_F(GPIOPinTest, writeGPIOPin)
{
    GPIO_RESET(PORT);

    writeGPIOPin(&pin, LOW);
    EXPECT_EQ(0x00U, PORT->ODRArray[0] & PIN);

    writeGPIOPin(&pin, HIGH);
    EXPECT_EQ(PIN, PORT->ODRArray[1] & PIN);
}

TEST_F(GPIOPinTest, writeGPIOPinValue)
{
    GPIO_RESET(PORT);

    writeGPIOPinValue(&pin, 0);
    EXPECT_EQ(0x00U, PORT->ODRArray[0] & PIN);

    writeGPIOPinValue(&pin, 1);
    EXPECT_EQ(PIN, PORT->ODRArray[1] & PIN);
}

TEST_F(GPIOPinTest, getGPIOPinPort)
{
    EXPECT_EQ(PORT, getGPIOPinPort(&pin));
}

TEST_F(GPIOPinTest, getGPIOPinPin)
{
    EXPECT_EQ(PIN, getGPIOPinPin(&pin));
}
