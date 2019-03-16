#include "button.h"

#define __DEFAULT_PRIORITY ((uint8_t)0)

// private method(s)
PRIVATE uint32_t getButtonEXTILine(Button * pThis);
PRIVATE uint8_t getButtonIRQChannel(Button * pThis);

PUBLIC Button newButton(GPIOPin pin, GPIOPinState clickState)
{
    Button button = {
        ._pin       = pin,
        ._statClick = clickState,

        ._semaphore = NULL, 

        .onClick    = defaultOnButtonClick
    };

    setupGPIOPin(&button._pin, INPUT_PULLUP);

    return button;
}

PUBLIC bool isButtonClicked(Button * pThis)
{
    BUTTON_DEBOUNCE();
    return readGPIOPin(&pThis->_pin) == pThis->_statClick;
}

PUBLIC bool isButtonInterruptEnable(Button * pThis)
{
    return pThis->_semaphore != NULL;
}

PUBLIC void setButtonInterrupt(Button * pThis, FunctionalState newState)
{
    // AFIO is used by other place, never disable it.
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    uint8_t portSource = getGPIOPinPortSource(&pThis->_pin);
    uint8_t pinSource = getGPIOPinPinSource(&pThis->_pin);
    GPIO_EXTILineConfig(portSource, pinSource);

    EXTITrigger_TypeDef trigger = pThis->_statClick ?
        EXTI_Trigger_Rising : EXTI_Trigger_Falling;
    EXTI_InitTypeDef EXTI_InitStructure = {
        .EXTI_Line      = getButtonEXTILine(pThis),
        .EXTI_Mode      = EXTI_Mode_Interrupt,
        .EXTI_Trigger   = trigger,
        .EXTI_LineCmd   = newState
    };

    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure = {
        .NVIC_IRQChannel                    = getButtonIRQChannel(pThis),
        .NVIC_IRQChannelPreemptionPriority  = __DEFAULT_PRIORITY,
        .NVIC_IRQChannelSubPriority         = __DEFAULT_PRIORITY,
        .NVIC_IRQChannelCmd                 = newState
    };

    NVIC_Init(&NVIC_InitStructure);

    if (newState == ENABLE) {
        vSemaphoreCreateBinary(pThis->_semaphore);
    }
}

PUBLIC VIRTUAL void defaultOnButtonClick(Button * pThis)
{
    // do nothing
}

PRIVATE uint32_t getButtonEXTILine(Button * pThis)
{
    return (uint32_t)pThis->_pin._pin;
}

PRIVATE uint8_t getButtonIRQChannel(Button * pThis)
{
    uint8_t irqChannel = 0;

    switch (pThis->_pin._pin) {
    case GPIO_Pin_0:
        irqChannel = EXTI0_IRQn;
        break;

    case GPIO_Pin_1:
        irqChannel = EXTI1_IRQn;
        break;

    case GPIO_Pin_2:
        irqChannel = EXTI2_IRQn;
        break;

    case GPIO_Pin_3:
        irqChannel = EXTI3_IRQn;
        break;

    case GPIO_Pin_4:
        irqChannel = EXTI4_IRQn;
        break;

    case GPIO_Pin_5:
    case GPIO_Pin_6:
    case GPIO_Pin_7:
    case GPIO_Pin_8:
    case GPIO_Pin_9:
        irqChannel = EXTI9_5_IRQn;
        break;

    case GPIO_Pin_10:
    case GPIO_Pin_11:
    case GPIO_Pin_12:
    case GPIO_Pin_13:
    case GPIO_Pin_14:
    case GPIO_Pin_15:
        irqChannel = EXTI15_10_IRQn;
        break;
    }

    return irqChannel;
}

PUBLIC STATIC void vButtonInterruptHandler(void * pButton)
{
    Button * button = (Button *)pButton;

    if (!isButtonInterruptEnable(button)) {
        vTaskDelete(NULL);
    }

    for (;;) {
        xSemaphoreTake(button->_semaphore, portMAX_DELAY);

        if (isButtonClicked(button)) {
            button->onClick(button);
        }
    }
}
