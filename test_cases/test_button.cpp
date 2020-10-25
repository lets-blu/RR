#include "button.h"
#include "gtest/gtest.h"

void onButtonClick(Button* button);

class ButtonTest : public ::testing::Test
{
public:
    static int onButtonClickCallCount;

protected:
    Button button;
    const uint16_t PIN = GPIO_PIN_8;
    GPIO_TypeDef * const PORT = GPIOA;

    void SetUp()
    {
        button = newButton(newGPIOPin(PORT, PIN), LOW);
        button.onClick = onButtonClick;
    }

    void TearDown()
    {
        // do nothing here
    }
};

int ButtonTest::onButtonClickCallCount = 0;

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
    ButtonTest::onButtonClickCallCount = 0;
    setButtonInterruptEnabled(&button, true);

    onButtonInterruptOccurred(&button);
    vButtonInterruptHandler(&button);
    EXPECT_EQ(1, ButtonTest::onButtonClickCallCount);
}

TEST_F(ButtonTest, isButtonInterruptEnabled)
{
    // test on setButtonInterruptEnabled
}

TEST_F(ButtonTest, setButtonInterruptEnabled)
{
    setButtonInterruptEnabled(&button, false);
    EXPECT_FALSE(isButtonInterruptEnabled(&button));

    setButtonInterruptEnabled(&button, true);
    EXPECT_TRUE(isButtonInterruptEnabled(&button));
}

void onButtonClick(Button * button)
{
    (void)button;
    ButtonTest::onButtonClickCallCount++;
}
