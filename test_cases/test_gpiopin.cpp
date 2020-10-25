#include "gpiopin.h"
#include "gtest/gtest.h"

class GPIOPinTest : public ::testing::Test
{
protected:
    GPIOPin pin;
    const uint16_t PIN = GPIO_PIN_13;
    GPIO_TypeDef * const PORT = GPIOC;

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

    setupGPIOPin(&pin, 0xFFFFFFFF);
    EXPECT_EQ(0, HAL_GPIO_InitCallCount);

    setupGPIOPin(&pin, OUTPUT);
    EXPECT_EQ(1, HAL_GPIO_InitCallCount);
}

TEST_F(GPIOPinTest, readGPIOPin)
{
    PORT->IDR &= ~PIN;
    EXPECT_EQ(LOW, readGPIOPin(&pin));

    PORT->IDR |= PIN;
    EXPECT_EQ(HIGH, readGPIOPin(&pin));
}

TEST_F(GPIOPinTest, writeGPIOPin)
{
    writeGPIOPin(&pin, LOW);
    EXPECT_EQ(0, PORT->ODR & PIN);

    writeGPIOPin(&pin, HIGH);
    EXPECT_EQ(PIN, PORT->ODR & PIN);
}

TEST_F(GPIOPinTest, writeGPIOPinValue)
{
    writeGPIOPinValue(&pin, 0);
    EXPECT_EQ(0, PORT->ODR & PIN);

    writeGPIOPinValue(&pin, 1);
    EXPECT_EQ(PIN, PORT->ODR & PIN);
}

TEST_F(GPIOPinTest, getGPIOPinPort)
{
    EXPECT_EQ(PORT, getGPIOPinPort(&pin));
}

TEST_F(GPIOPinTest, getGPIOPinPin)
{
    EXPECT_EQ(PIN, getGPIOPinPin(&pin));
}
