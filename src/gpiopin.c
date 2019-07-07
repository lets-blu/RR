#include "gpiopin.h"

PUBLIC GPIOPin newGPIOPin(GPIO_TypeDef * GPIOx, uint16_t GPIO_PIN_x)
{
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_PIN_x));

    GPIOPin pin = 
    {
        ._port  = GPIOx, 
        ._pin   = GPIO_PIN_x
    };

    return pin;
}

PUBLIC void setupGPIOPin(GPIOPin * pThis, GPIOPinMode mode)
{
    assert_param(IS_GPIO_MODE(mode & ~PULLUP));

    // enable GPIO port clock
    if (pThis->_port == GPIOA)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    else if (pThis->_port == GPIOB)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    else
    {
        if (pThis->_port == GPIOC)
        {
            __HAL_RCC_GPIOC_CLK_ENABLE();
        }
    }

    // initialize GPIO pin
    GPIO_InitTypeDef init =
    {
        .Pin    = pThis->_pin,
        .Mode   = mode & ~PULLUP,
        .Pull   = (mode & PULLUP) ? GPIO_PULLUP : GPIO_NOPULL,
        .Speed  = GPIO_SPEED_FREQ_LOW
    };

    HAL_GPIO_Init(pThis->_port, &init);
}

PUBLIC GPIOPinState readGPIOPin(GPIOPin * pThis)
{
    return HAL_GPIO_ReadPin(pThis->_port, pThis->_pin);
}

PUBLIC void writeGPIOPin(GPIOPin * pThis, GPIOPinState state)
{
    HAL_GPIO_WritePin(pThis->_port, pThis->_pin, state);
}
