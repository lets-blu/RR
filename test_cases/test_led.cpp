#include "led.h"
#include "gtest/gtest.h"

class LEDTest : public ::testing::Test
{
protected:
    LED led;
    const uint16_t PIN = GPIO_PIN_13;
    GPIO_TypeDef * const PORT = GPIOC;

    void SetUp()
    {
        led = newLED(newGPIOPin(PORT, PIN), LOW);
    }

    void TearDown()
    {
        // do nothing here
    }
};

TEST_F(LEDTest, turnOnLED)
{
    turnOnLED(&led);
    EXPECT_EQ(0, PORT->ODR & PIN);
}

TEST_F(LEDTest, turnOffLED)
{
    turnOffLED(&led);
    EXPECT_EQ(PIN, PORT->ODR & PIN);
}
