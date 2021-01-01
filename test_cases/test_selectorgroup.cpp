#include "gtest/gtest.h"
#include "selectorgroup.h"

class SelectorGroupTest : public ::testing::Test
{
protected:
    const uint8_t BIT = 7;
    static const int SELECTOR_COUNT = 2;
    static const int OBSERVER_COUNT = 2;

    SelectorGroup selectorGroup;
    DataSelector dataSelectors[SELECTOR_COUNT];
    ChainedObserver chainedObservers[OBSERVER_COUNT];

    GPIO_TypeDef * const PORT = GPIOB;
    const uint16_t SCAN_PIN = GPIO_PIN_3;
    const uint16_t ADDRESS_PINS = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;

    void SetUp()
    {
        GPIOPin scanPin = newGPIOPin(PORT, SCAN_PIN);
        GPIOPin addressPins = newGPIOPin(PORT, ADDRESS_PINS);

        // construct Selector Group
        selectorGroup = newSelectorGroup(scanPin, addressPins);

        // construct Data Selector
        for (int i = 0; i < SELECTOR_COUNT; i++)
        {
            dataSelectors[i] = newDataSelector(i * 8, i * 8 + 7);
            addSelectorGroupSelector(&selectorGroup, &dataSelectors[i]);
        }

        // construct Chained Observer
        for (int i = 0; i < OBSERVER_COUNT; i++)
        {
            chainedObservers[i] = newChainedObserver();
            attachSelectorGroupObserver(&selectorGroup, &chainedObservers[i].observer);
        }
    }

    void TearDown()
    {
        // do nothing here
    }
};

TEST_F(SelectorGroupTest, addSelectorGroupSelector)
{
    EXPECT_EQ(&dataSelectors[SELECTOR_COUNT - 1], selectorGroup._selectors);

    for (int i = 0; i < SELECTOR_COUNT - 1; i++)
    {
        EXPECT_EQ(&dataSelectors[i], dataSelectors[i + 1]._next);
    }
}

TEST_F(SelectorGroupTest, getSelectorGroupMessage)
{

}

TEST_F(SelectorGroupTest, isSelectorGroupScanEnabled)
{
    // test on setSelectorGroupScanEnabled
}

TEST_F(SelectorGroupTest, setSelectorGroupScanEnabled)
{
    setSelectorGroupScanEnabled(&selectorGroup, false);
    setSelectorGroupScanEnabled(&selectorGroup, false);
    EXPECT_FALSE(isSelectorGroupScanEnabled(&selectorGroup));

    setSelectorGroupScanEnabled(&selectorGroup, true);
    setSelectorGroupScanEnabled(&selectorGroup, true);
    EXPECT_TRUE(isSelectorGroupScanEnabled(&selectorGroup));
}
