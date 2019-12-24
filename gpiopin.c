#include "gpiopin.h"

PUBLIC GPIOPin newGPIOPin(GPIO_TypeDef * GPIOx, uint16_t GPIO_PIN_x)
{
    assert(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert(IS_GPIO_PIN(GPIO_PIN_x));

    GPIOPin pin = {
        ._port  = GPIOx, 
        ._pin   = GPIO_PIN_x
    };

    return pin;
}

PUBLIC void setupGPIOPin(GPIOPin * pThis, GPIOPinMode mode)
{
    if (!IS_GPIO_MODE(mode & ~PULLUP))
    {
        return;
    }

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
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }

    // initialize GPIO pin
    GPIO_InitTypeDef init = {
        .Pin    = pThis->_pin,
        .Mode   = mode & ~PULLUP,
        .Pull   = ((mode & PULLUP) == PULLUP) ? GPIO_PULLUP : GPIO_NOPULL,
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

PUBLIC GPIO_TypeDef * getGPIOPinPort(GPIOPin * pThis)
{
    return pThis->_port;
}

PUBLIC uint16_t getGPIOPinPin(GPIOPin * pThis)
{
    return pThis->_pin;
}

PUBLIC uint8_t getGPIOPinPinOffset(GPIOPin * pThis)
{
    uint8_t offset = 0;

    for (uint16_t i = 0x0001; (pThis->_pin & i) == 0x0000; i <<= 1, offset++)
    {
        // do nothing here
    }

    return offset;
}
