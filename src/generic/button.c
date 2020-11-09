#include "button.h"

#define BUTTON_PREEMPT_PRIORITY     5U
#define BUTTON_SUB_PRIORITY         0U

#define BUTTON_DEBOUNCE()           osDelay(50)

#ifdef UNIT_TEST
extern int defaultOnButtonClickCallCount;
#endif // UNIT_TEST

// Private method(s)
PRIVATE IRQn_Type getButtonIRQn(Button * pThis);

PRIVATE void enableButtonInterrupt(Button * pThis);
PRIVATE void disableButtonInterrupt(Button * pThis);

PUBLIC Button newButton(GPIOPin pin, GPIOPinState clickState)
{
    Button button = {
        ._pin                   = pin,
        ._clickState            = clickState,

        ._interruptHandler      = NULL,
        ._interruptSemaphore    = NULL,

        .onClick                = defaultOnButtonClick
    };

    // initialize button's pin
    setupGPIOPin(&pin, (clickState == LOW) ? INPUT_PULLUP : INPUT);

    return button;
}

PUBLIC bool isButtonClicked(Button * pThis)
{
    BUTTON_DEBOUNCE();
    GPIOPinState currentState = readGPIOPin(&pThis->_pin);

    BUTTON_DEBOUNCE();
    return (currentState == pThis->_clickState);
}

PUBLIC void onButtonInterruptOccurred(Button * pThis)
{
    osSemaphoreRelease(pThis->_interruptSemaphore);
}

PUBLIC bool isButtonInterruptEnabled(Button * pThis)
{
    return ((pThis->_interruptHandler != NULL)
        && (osThreadGetState(pThis->_interruptHandler) != osThreadDeleted));
}

PUBLIC void setButtonInterruptEnabled(Button * pThis, bool enabled)
{
    if (enabled)
    {
        enableButtonInterrupt(pThis);
    }
    else
    {
        disableButtonInterrupt(pThis);
    }
}

PUBLIC VIRTUAL void defaultOnButtonClick(Button * pThis)
{
    (void)pThis;
#ifdef UNIT_TEST
    defaultOnButtonClickCallCount++;
#endif // UNIT_TEST
}

PRIVATE IRQn_Type getButtonIRQn(Button * pThis)
{
    uint16_t pin = getGPIOPinPin(&pThis->_pin);

    if (pin == GPIO_PIN_0)
    {
        return EXTI0_IRQn;
    }
    else if (pin == GPIO_PIN_1)
    {
        return EXTI1_IRQn;
    }
    else if (pin == GPIO_PIN_2)
    {
        return EXTI2_IRQn;
    }
    else if (pin == GPIO_PIN_3)
    {
        return EXTI3_IRQn;
    }
    else if (pin == GPIO_PIN_4)
    {
        return EXTI4_IRQn;
    }
    else if (GPIO_PIN_5 <= pin && pin <= GPIO_PIN_9)
    {
        return EXTI9_5_IRQn;
    }
    else
    {
        return EXTI15_10_IRQn;
    }
}

PRIVATE void enableButtonInterrupt(Button * pThis)
{
    GPIOPinMode mode = INTERRUPT_RISING;

    // if button interrupt enabled, there is no need to enable again
    if (isButtonInterruptEnabled(pThis))
    {
        return;
    }

    // initialize binary semaphore for interrupt handler
    if (pThis->_interruptSemaphore == NULL)
    {
        osSemaphoreDef(semaphore);
        pThis->_interruptSemaphore = osSemaphoreCreate(osSemaphore(semaphore), 1);
    }

    // get interrupt mode
    if (pThis->_clickState == LOW)
    {
        mode = INTERRUPT_FALLING | PULLUP;
    }

    // enable button interrupt
    setupGPIOPin(&pThis->_pin, mode);

    IRQn_Type irqn = getButtonIRQn(pThis);
    HAL_NVIC_SetPriority(irqn, BUTTON_PREEMPT_PRIORITY, BUTTON_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(irqn);

    osSemaphoreWait(pThis->_interruptSemaphore, 0); // clear interrupt semaphore
    osThreadDef(handler, vButtonInterruptHandler, osPriorityRealtime, 0, 128);
    pThis->_interruptHandler = osThreadCreate(osThread(handler), pThis);
}

PRIVATE void disableButtonInterrupt(Button * pThis)
{
    GPIOPinMode mode = INPUT;

    // if button interrupt disabled, there is no need to disable again
    if (!isButtonInterruptEnabled(pThis))
    {
        return;
    }

    // get button input mode
    if (pThis->_clickState == LOW)
    {
        mode = INPUT_PULLUP;
    }

    // disable button interrupt
    setupGPIOPin(&pThis->_pin, mode);
    HAL_NVIC_DisableIRQ(getButtonIRQn(pThis));
    osThreadTerminate(pThis->_interruptHandler);
}

PUBLIC STATIC void vButtonInterruptHandler(void const * argument)
{
    Button * button = (Button *)argument;

    LOOP
    {
        if ((osSemaphoreWait(button->_interruptSemaphore, osWaitForever) != -1))
        {
            if (isButtonClicked(button))
            {
                button->onClick(button);
            }
        }
    }
}
