#include "button.h"

#define __BUTTON_PREEMPT_PRIORITY   ((uint32_t)5)
#define __BUTTON_SUB_PRIORITY       ((uint32_t)0)

#define __BUTTON_DEBOUNCE()         osDelay(50)

// private method(s)
PRIVATE void enableButtonInterrupt(Button * pThis);
PRIVATE void disableButtonInterrupt(Button * pThis);
PRIVATE IRQn_Type getButtonIRQn(Button * pThis);

PRIVATE STATIC void iButtonInterruptHandler(void const * argument);

PUBLIC Button newButton(GPIOPin pin, GPIOPinState clickState)
{
    Button button = 
    {
        ._pin                   = pin, 
        ._clickState            = clickState, 

        ._interruptHandler      = NULL, 
        ._interruptSemaphore    = NULL, 

        .onClick                = defaultOnButtonClick
    };

    // initialize binary semaphore for interrupt handler
    osSemaphoreDef(semaphore);
    button._interruptSemaphore = osSemaphoreCreate(osSemaphore(semaphore), 1);

    // initialize button's pin
    setupGPIOPin(&pin, (clickState == LOW) ? INPUT_PULLUP : INPUT);

    return button;
}

PUBLIC bool isButtonClicked(Button * pThis)
{
    __BUTTON_DEBOUNCE();
    GPIOPinState currentState = readGPIOPin(&pThis->_pin);

    __BUTTON_DEBOUNCE();

    return (currentState == pThis->_clickState);
}

PUBLIC bool isButtonInterruptEnabled(Button * pThis)
{
    return ((pThis->_interruptHandler != NULL)
        && (osThreadGetState(pThis->_interruptHandler) != osThreadDeleted));
}

PUBLIC void setButtonInterrupt(Button * pThis, FunctionalState state)
{
    if (state != DISABLE)
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
    // do nothing here
}

PRIVATE void enableButtonInterrupt(Button * pThis)
{
    // if button interrupt enabled, there is no need to enable again
    if (isButtonInterruptEnabled(pThis))
    {
        return;
    }

    // get IRQ number and interrupt mode
    GPIOPinMode mode = INTERRUPT_CHANGE;
    IRQn_Type irq = getButtonIRQn(pThis);

    if (pThis->_clickState == LOW)
    {
        mode = INTERRUPT_FALLING | PULLUP;
    }
    else
    {
        mode = INTERRUPT_RISING;
    }

    // enable button interrupt
    setupGPIOPin(&pThis->_pin, mode);

    HAL_NVIC_SetPriority(irq, __BUTTON_PREEMPT_PRIORITY, __BUTTON_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(irq);

    osSemaphoreWait(pThis->_interruptSemaphore, 0);

    osThreadDef(handler, iButtonInterruptHandler, osPriorityRealtime, 0, 128);
    pThis->_interruptHandler = osThreadCreate(osThread(handler), pThis);
}

PRIVATE void disableButtonInterrupt(Button * pThis)
{
    // if button interrupt disabled, there is no need to disable again
    if (!isButtonInterruptEnabled(pThis))
    {
        return;
    }

    // get button input mode
    GPIOPinMode mode = INPUT;

    if (pThis->_clickState == LOW)
    {
        mode = INPUT_PULLUP;
    }

    // disable button interrupt
    setupGPIOPin(&pThis->_pin, mode);

    HAL_NVIC_DisableIRQ(getButtonIRQn(pThis));
    osThreadTerminate(pThis->_interruptHandler);
}

PRIVATE IRQn_Type getButtonIRQn(Button * pThis)
{
    IRQn_Type irq = (IRQn_Type)0;
    uint16_t pin = getGPIOPinPin(&pThis->_pin);

    if (pin == GPIO_PIN_0)
    {
        irq = EXTI0_IRQn;
    }
    else if (pin == GPIO_PIN_1)
    {
        irq = EXTI1_IRQn;
    }
    else if (pin == GPIO_PIN_2)
    {
        irq = EXTI2_IRQn;
    }
    else if (pin == GPIO_PIN_3)
    {
        irq = EXTI3_IRQn;
    }
    else if (pin == GPIO_PIN_4)
    {
        irq = EXTI4_IRQn;
    }
    else if (GPIO_PIN_5 <= pin && pin <= GPIO_PIN_9)
    {
        irq = EXTI9_5_IRQn;
    }
    else
    {
        if (GPIO_PIN_10 <= pin && pin <= GPIO_PIN_15)
        {
            irq = EXTI15_10_IRQn;
        }
    }

    return irq;
}

PRIVATE STATIC void iButtonInterruptHandler(void const * argument)
{
    Button * button = (Button *)argument;

    for (;;)
    {
        osSemaphoreWait(button->_interruptSemaphore, osWaitForever);

        if (isButtonClicked(button))
        {
            button->onClick(button);
        }
    }
}
