#include "gpiopin.h"

PUBLIC GPIOPin newGPIOPin(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin_x) {
    GPIOPin pin = {
        ._port = GPIOx,
        ._pin = GPIO_Pin_x
    };

    if (GPIOx == GPIOA) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    }
    else if (GPIOx == GPIOB) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    }
    else if (GPIOx == GPIOC) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    }
    else if (GPIOx == GPIOD) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    }
    else if (GPIOx == GPIOE) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    }
    else if (GPIOx == GPIOF) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    }
    else {
        if (GPIOx == GPIOG) {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
        }
    }

    return pin;
}

PUBLIC void configGPIOPin(GPIOPin * pThis,
    GPIOSpeed_TypeDef GPIO_Speed, GPIOMode_TypeDef GPIO_Mode) {
    GPIO_InitTypeDef GPIO_InitStructure = {
        .GPIO_Pin = pThis->_pin,
        .GPIO_Speed = GPIO_Speed,
        .GPIO_Mode = GPIO_Mode
    };

    GPIO_Init(pThis->_port, &GPIO_InitStructure);
}

PUBLIC GPIOPinState readGPIOPin(GPIOPin * pThis) {
    return (GPIOPinState)GPIO_ReadInputDataBit(pThis->_port, pThis->_pin);
}

PUBLIC void writeGPIOPin(GPIOPin * pThis, GPIOPinState state) {
    if (state == LOW) {
        GPIO_ResetBits(pThis->_port, pThis->_pin);
    }
    else {
        GPIO_SetBits(pThis->_port, pThis->_pin);
    }
}

PUBLIC void getGPIOPinPortSource(GPIOPin * pThis, uint8_t * portSource) {
    if (pThis->_port == GPIOA) {
        *portSource = GPIO_PortSourceGPIOA;
    }
    else if (pThis->_port == GPIOB) {
        *portSource = GPIO_PortSourceGPIOB;
    }
    else if (pThis->_port == GPIOC) {
        *portSource = GPIO_PortSourceGPIOC;
    }
    else if (pThis->_port == GPIOD) {
        *portSource = GPIO_PortSourceGPIOD;
    }
    else if (pThis->_port == GPIOE) {
        *portSource = GPIO_PortSourceGPIOE;
    }
    else if (pThis->_port == GPIOF) {
        *portSource = GPIO_PortSourceGPIOF;
    }
    else {
        if (pThis->_port == GPIOG) {
            *portSource = GPIO_PortSourceGPIOG;
        }
    }
}

PUBLIC void getGPIOPinPinSource(GPIOPin * pThis, uint8_t * pinSource) {
    *pinSource = 0;
    for (uint16_t pin = pThis->_pin; pin; *pinSource++, pin >>= 1);
}
