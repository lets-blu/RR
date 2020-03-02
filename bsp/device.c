#include "device.h"

#define LED_DEVICE_NUM              5
#define BUTTON_DEVICE_NUM           4

#define SHTREG_DEVICE_NUM           2
#define REGGRP_DEVICE_NUM           1
#define DATSEL_DEVICE_NUM           2
#define SELGRP_DEVICE_NUM           1

#define WATCHDOG_DEVICE_NUM         1
#define SELGRP_DEVICE_CALLBACK_NUM  1

PRIVATE STATIC LED ledDevice[LED_DEVICE_NUM];
PRIVATE STATIC Button buttonDevice[BUTTON_DEVICE_NUM];

PRIVATE STATIC ShiftRegister shiftRegisterDevice[SHTREG_DEVICE_NUM];
PRIVATE STATIC RegisterGroup registerGroupDevice[REGGRP_DEVICE_NUM];
PRIVATE STATIC DataSelector dataSelectorDevice[DATSEL_DEVICE_NUM];
PRIVATE STATIC SelectorGroup selectorGroupDevice[SELGRP_DEVICE_NUM];

PRIVATE STATIC Watchdog watchdogDevice[WATCHDOG_DEVICE_NUM];
PRIVATE STATIC ChainedObserver deviceObserver[SELGRP_DEVICE_CALLBACK_NUM];

PUBLIC STATIC void initializeAllDevices(void)
{
    RegisterGroupPins pins = {
        .oePin  = newGPIOPin(GPIOA, GPIO_PIN_4),
        .serPin = newGPIOPin(GPIOA, GPIO_PIN_7),
        .sckPin = newGPIOPin(GPIOA, GPIO_PIN_5),
        .rckPin = newGPIOPin(GPIOA, GPIO_PIN_3)
    };

    ledDevice[0] = newLED(newGPIOPin(GPIOB, GPIO_PIN_8), LOW);
    ledDevice[1] = newLED(newGPIOPin(GPIOB, GPIO_PIN_9), LOW);
    ledDevice[2] = newLED(newGPIOPin(GPIOC, GPIO_PIN_13), LOW);
    ledDevice[3] = newLED(newGPIOPin(GPIOC, GPIO_PIN_14), LOW);
    ledDevice[4] = newLED(newGPIOPin(GPIOC, GPIO_PIN_15), LOW);

    buttonDevice[0] = newButton(newGPIOPin(GPIOA, GPIO_PIN_8), LOW);
    buttonDevice[1] = newButton(newGPIOPin(GPIOA, GPIO_PIN_9), LOW);
    buttonDevice[2] = newButton(newGPIOPin(GPIOA, GPIO_PIN_10), LOW);
    buttonDevice[3] = newButton(newGPIOPin(GPIOA, GPIO_PIN_11), LOW);

    shiftRegisterDevice[0] = newShiftRegister();
    shiftRegisterDevice[1] = newShiftRegister();
    registerGroupDevice[0] = newRegisterGroup(pins);
    addRegisterToGroup(&registerGroupDevice[0], &shiftRegisterDevice[0]);
    addRegisterToGroup(&registerGroupDevice[0], &shiftRegisterDevice[1]);

    dataSelectorDevice[0] = newDataSelector(0, 7);
    dataSelectorDevice[1] = newDataSelector(8, 15);
    selectorGroupDevice[0] = newSelectorGroup(newGPIOPin(GPIOB, GPIO_PIN_3), 
        newGPIOPin(GPIOB, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7));
    addSelectorToGroup(&selectorGroupDevice[0], &dataSelectorDevice[0]);
    addSelectorToGroup(&selectorGroupDevice[0], &dataSelectorDevice[1]);

    watchdogDevice[0] = newWatchdog();
}

PUBLIC STATIC LED * getLEDDevice(const char * name)
{
    if (strcmp(name, "LED1") == 0)
    {
        return &ledDevice[0];
    }
    else if (strcmp(name, "LED2") == 0)
    {
        return &ledDevice[1];
    }
    else if (strcmp(name, "LED3") == 0)
    {
        return &ledDevice[2];
    }
    else if (strcmp(name, "LED4") == 0)
    {
        return &ledDevice[3];
    }
    else if (strcmp(name, "LED5") == 0)
    {
        return &ledDevice[4];
    }
    else
    {
        return NULL;
    }
}

PUBLIC STATIC Button * getButtonDevice(const char * name)
{
    if (strcmp(name, "KEY1") == 0)
    {
        return &buttonDevice[0];
    }
    else if (strcmp(name, "KEY2") == 0)
    {
        return &buttonDevice[1];
    }
    else if (strcmp(name, "KEY3") == 0)
    {
        return &buttonDevice[2];
    }
    else if (strcmp(name, "KEY4") == 0)
    {
        return &buttonDevice[3];
    }
    else
    {
        return NULL;
    }
}

PUBLIC STATIC RegisterGroup * getRegisterGroupDevice(void)
{
    return &registerGroupDevice[0];
}

PUBLIC STATIC SelectorGroup * getSelectorGroupDevice(void)
{
    return &selectorGroupDevice[0];
}

PUBLIC STATIC void enableWatchdogDevice(uint16_t timeout)
{
    enableWatchdog(&watchdogDevice[0], timeout);
}

PUBLIC STATIC void reloadWatchdogDevice(void)
{
    reloadWatchdog(&watchdogDevice[0]);
}

PUBLIC STATIC void setButtonDeviceClickCallback(Button * button, 
    on_button_click_fp callback)
{
    if (callback != NULL)
    {
        button->onClick = callback;
    }
    else
    {
        button->onClick = defaultOnButtonClick;
    }
}

PUBLIC STATIC bool addSelectorGroupDeviceScanCallback(
    SelectorGroup * selectorGroup, update_observer_fp callback)
{
    for (uint8_t i = 0; i < SELGRP_DEVICE_CALLBACK_NUM; i++)
    {
        struct IObserver * observer = &deviceObserver[i].observer;

        if (observer->update == NULL)
        {
            observer->update = callback;
            registerObserverToSelectorGroup(selectorGroup, observer);

            return true;
        }
    }

    return false;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
        case GPIO_PIN_8:
        {
            onButtonInterruptOccurred(&buttonDevice[0]);
            break;
        }

        case GPIO_PIN_9:
        {
            onButtonInterruptOccurred(&buttonDevice[1]);
            break;
        }

        case GPIO_PIN_10:
        {
            onButtonInterruptOccurred(&buttonDevice[2]);
            break;
        }

        case GPIO_PIN_11:
        {
            onButtonInterruptOccurred(&buttonDevice[3]);
            break;
        }

        default:
        {
            break;
        }
    }
}
