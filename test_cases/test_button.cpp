#include "button.h"
#include "gtest/gtest.h"

class ButtonTest : public ::testing::Test
{
protected:
    Button button;
    const uint16_t PIN = GPIO_PIN_10; // GPIO_PIN_8
    GPIO_TypeDef * const PORT = GPIOA;

    void SetUp()
    {
        button = newButton(newGPIOPin(PORT, PIN), LOW);
    }

    void TearDown()
    {
        // do nothing here
    }
};

int defaultOnButtonClickCallCount;

TEST_F(ButtonTest, isButtonClicked)
{
    GPIO_RESET(PORT);

    PORT->IDRArray[0] |= PIN;
    EXPECT_FALSE(isButtonClicked(&button));

    PORT->IDRArray[1] &= ~PIN;
    EXPECT_TRUE(isButtonClicked(&button));
}

TEST_F(ButtonTest, onButtonInterruptOccurred)
{
    GPIO_RESET(PORT);
    PORT->IDRArray[0] &= ~PIN;
    defaultOnButtonClickCallCount = 0;

    setButtonInterruptEnabled(&button, true);
    onButtonInterruptOccurred(&button);
    vButtonInterruptHandler(&button);
    EXPECT_EQ(1, defaultOnButtonClickCallCount);
}

TEST_F(ButtonTest, isButtonInterruptEnabled)
{
    // test on setButtonInterruptEnabled
}

TEST_F(ButtonTest, setButtonInterruptEnabled)
{
    HAL_NVIC_SetPriorityCallCount = 0;
    HAL_NVIC_EnableIRQCallCount = 0;
    HAL_NVIC_DisableIRQCallCount = 0;

    setButtonInterruptEnabled(&button, false);
    setButtonInterruptEnabled(&button, true);
    EXPECT_EQ(1, HAL_NVIC_SetPriorityCallCount);
    EXPECT_EQ(1, HAL_NVIC_EnableIRQCallCount);
    EXPECT_TRUE(isButtonInterruptEnabled(&button));

    setButtonInterruptEnabled(&button, true);
    setButtonInterruptEnabled(&button, false);
    EXPECT_EQ(1, HAL_NVIC_DisableIRQCallCount);
    EXPECT_FALSE(isButtonInterruptEnabled(&button));
}
