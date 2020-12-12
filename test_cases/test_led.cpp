#include "gtest/gtest.h"
#include "led.h"

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
    RESET_GPIO_INSTANCE(PORT);

    turnOnLED(&led);
    EXPECT_EQ(0x00U, PORT->ODRArray[0] & PIN);
}

TEST_F(LEDTest, turnOffLED)
{
    RESET_GPIO_INSTANCE(PORT);

    turnOffLED(&led);
    EXPECT_EQ(PIN, PORT->ODRArray[0] & PIN);
}
