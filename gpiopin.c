#include "gpiopin.h"

#define __DEFAULT_SPEED GPIO_Speed_2MHz

PUBLIC GPIOPin newGPIOPin(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin_x)
{
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin_x));

    GPIOPin pin = {
        ._port  = GPIOx,
        ._pin   = GPIO_Pin_x
    };

    return pin;
}

PUBLIC void setupGPIOPin(GPIOPin * pThis, GPIOPinMode mode)
{
    GPIO_InitTypeDef GPIO_InitStructure = {
        .GPIO_Pin   = pThis->_pin,
        .GPIO_Speed = __DEFAULT_SPEED,
        .GPIO_Mode  = mode
    };

    if (pThis->_port == GPIOA) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    }
    else if (pThis->_port == GPIOB) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    }
    else if (pThis->_port == GPIOC) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    }
    else if (pThis->_port == GPIOD) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    }
    else if (pThis->_port == GPIOE) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    }
    else if (pThis->_port == GPIOF) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    }
    else {
        if (pThis->_port == GPIOG) {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
        }
    }

    GPIO_Init(pThis->_port, &GPIO_InitStructure);
}

PUBLIC GPIOPinState readGPIOPin(GPIOPin * pThis)
{
    return (GPIOPinState)GPIO_ReadInputDataBit(pThis->_port, pThis->_pin);
}

PUBLIC void writeGPIOPin(GPIOPin * pThis, GPIOPinState state)
{
    GPIO_WriteBit(pThis->_port, pThis->_pin, state);
}

PUBLIC uint8_t getGPIOPinPortSource(GPIOPin * pThis)
{
    uint8_t portSource = 0;

    if (pThis->_port == GPIOA) {
        portSource = GPIO_PortSourceGPIOA;
    }
    else if (pThis->_port == GPIOB) {
        portSource = GPIO_PortSourceGPIOB;
    }
    else if (pThis->_port == GPIOC) {
        portSource = GPIO_PortSourceGPIOC;
    }
    else if (pThis->_port == GPIOD) {
        portSource = GPIO_PortSourceGPIOD;
    }
    else if (pThis->_port == GPIOE) {
        portSource = GPIO_PortSourceGPIOE;
    }
    else if (pThis->_port == GPIOF) {
        portSource = GPIO_PortSourceGPIOF;
    }
    else {
        if (pThis->_port == GPIOG) {
            portSource = GPIO_PortSourceGPIOG;
        }
    }

    return portSource;
}

PUBLIC uint8_t getGPIOPinPinSource(GPIOPin * pThis)
{
    uint8_t pinSource = 0;

    for (uint16_t pin = pThis->_pin; pin != 0x00; ++pinSource, pin >>= 1) {
        ;// do nothing
    }

    return pinSource;
}
