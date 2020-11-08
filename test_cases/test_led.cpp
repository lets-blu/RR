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
    GPIO_RESET(PORT);
    turnOnLED(&led);
    EXPECT_EQ(0x00U, PORT->ODRArray[0] & PIN);
}

TEST_F(LEDTest, turnOffLED)
{
    GPIO_RESET(PORT);
    turnOffLED(&led);
    EXPECT_EQ(PIN, PORT->ODRArray[0] & PIN);
}
