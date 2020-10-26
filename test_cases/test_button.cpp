#include "button.h"
#include "gtest/gtest.h"

class ButtonTest : public ::testing::Test
{
public:
    static int defaultOnButtonClickCallCount;

protected:
    Button button;
    const uint16_t PIN = GPIO_PIN_8;
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

int ButtonTest::defaultOnButtonClickCallCount = 0;

TEST_F(ButtonTest, isButtonClicked)
{
    PORT->IDR |= PIN;
    EXPECT_FALSE(isButtonClicked(&button));

    PORT->IDR &= ~PIN;
    EXPECT_TRUE(isButtonClicked(&button));
}

TEST_F(ButtonTest, onButtonInterruptOccurred)
{
    PORT->IDR &= ~PIN;
    ButtonTest::defaultOnButtonClickCallCount = 0;

    setButtonInterruptEnabled(&button, true);

    onButtonInterruptOccurred(&button);
    vButtonInterruptHandler(&button);
    EXPECT_EQ(1, ButtonTest::defaultOnButtonClickCallCount);
}

TEST_F(ButtonTest, isButtonInterruptEnabled)
{
    // test on setButtonInterruptEnabled
}

TEST_F(ButtonTest, setButtonInterruptEnabled)
{
    setButtonInterruptEnabled(&button, false);
    setButtonInterruptEnabled(&button, true);
    EXPECT_TRUE(isButtonInterruptEnabled(&button));

    setButtonInterruptEnabled(&button, true);
    setButtonInterruptEnabled(&button, false);
    EXPECT_FALSE(isButtonInterruptEnabled(&button));
}

PUBLIC VIRTUAL void defaultOnButtonClick(Button * pThis)
{
    (void)pThis;
    ButtonTest::defaultOnButtonClickCallCount++;
}
