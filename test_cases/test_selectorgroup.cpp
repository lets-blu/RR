#include "gtest/gtest.h"
#include "selectorgroup.h"

class SelectorGroupTest : public ::testing::Test
{
public:
    static const uint8_t ADDRESS = 7;
    static const GPIOPinState STATE = HIGH;

protected:
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
        void updateTestChainedObserver(ChainedObserver * observer, ISubject * subject);

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
            chainedObservers[i].observer.update
                = (update_observer_fp)updateTestChainedObserver;
            attachSelectorGroupObserver(&selectorGroup, &chainedObservers[i].observer);
        }
    }

    void TearDown()
    {
        deleteSelectorGroup(&selectorGroup);
    }

    int getInputIndex(uint8_t address)
    {
        int location = OBSERVER_COUNT - (address / 8) - 1;
        return location * 8 + address % 8;
    }
};

static int updateTestChainedObserverMatchCount;

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
    // test on vScanSelectorGroupThread
}

TEST_F(SelectorGroupTest, isSelectorGroupScanEnabled)
{
    // test on setSelectorGroupScanEnabled
}

TEST_F(SelectorGroupTest, setSelectorGroupScanEnabled)
{
    setSelectorGroupScanEnabled(&selectorGroup, false);
    setSelectorGroupScanEnabled(&selectorGroup, true);
    EXPECT_TRUE(isSelectorGroupScanEnabled(&selectorGroup));

    setSelectorGroupScanEnabled(&selectorGroup, true);
    setSelectorGroupScanEnabled(&selectorGroup, false);
    EXPECT_FALSE(isSelectorGroupScanEnabled(&selectorGroup));
}

TEST_F(SelectorGroupTest, attachSelectorGroupObserver)
{
    EXPECT_EQ(&chainedObservers[OBSERVER_COUNT - 1], selectorGroup._observers);

    for (int i = 0; i < OBSERVER_COUNT - 1; i++)
    {
        EXPECT_EQ(&chainedObservers[i], chainedObservers[i + 1]._next);
    }
}

TEST_F(SelectorGroupTest, detachSelectorGroupObserver)
{
    for (int i = 0; i < OBSERVER_COUNT - 1; i++)
    {
        detachSelectorGroupObserver(&selectorGroup, &chainedObservers[i].observer);
        EXPECT_EQ(NULL, chainedObservers[i + 1]._next);
    }

    detachSelectorGroupObserver(
        &selectorGroup, &chainedObservers[OBSERVER_COUNT - 1].observer);
    EXPECT_EQ(NULL, selectorGroup._observers);
}

TEST_F(SelectorGroupTest, notifySelectorGroupObservers)
{
    // test on vScanSelectorGroupThread
}

TEST_F(SelectorGroupTest, vScanSelectorGroupThread)
{
    int count = OBSERVER_COUNT;
    int index = getInputIndex(ADDRESS);

    RESET_GPIO_INSTANCE(PORT);
    updateTestChainedObserverMatchCount = 0;
    setSelectorGroupScanEnabled(&selectorGroup, true);

    PORT->IDRArray[index] |= SCAN_PIN;
    vScanSelectorGroupThread(&selectorGroup);
    EXPECT_EQ(count, updateTestChainedObserverMatchCount);
}

void updateTestChainedObserver(ChainedObserver * observer, ISubject * subject)
{
    (void)observer;
    SelectorMessage message = getSelectorGroupMessage((SelectorGroup *)subject);

    if ((message.address == SelectorGroupTest::ADDRESS)
        && (message.state == SelectorGroupTest::STATE))
    {
        updateTestChainedObserverMatchCount++;
    }
}
