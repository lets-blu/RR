#include "gpiopin.h"

PUBLIC GPIOPin newGPIOPin(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin_x) {
    GPIOPin pin = {
        ._port = GPIOx, 
        ._pin = GPIO_Pin_x
    };
    
    if (GPIOx == GPIOA) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    } else if (GPIOx == GPIOB) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    } else if (GPIOx == GPIOC) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    } else if (GPIOx == GPIOD) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    } else if (GPIOx == GPIOE) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    } else if (GPIOx == GPIOF) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    } else {
        if (GPIOx == GPIOG) {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
        }
    }

    return pin;
}

PUBLIC void configGPIOPin(GPIOPin * this, 
        GPIOSpeed_TypeDef GPIO_Speed, GPIOMode_TypeDef GPIO_Mode) {
    GPIO_InitTypeDef GPIO_InitStructure = {
        .GPIO_Pin   = this->_pin, 
        .GPIO_Speed = GPIO_Speed, 
        .GPIO_Mode  = GPIO_Mode
    }; 

    GPIO_Init(this->_port, &GPIO_InitStructure);
}
				
PUBLIC GPIOPinState readGPIOPin(GPIOPin * this) {
    return (GPIOPinState)GPIO_ReadInputDataBit(this->_port, this->_pin);
}

PUBLIC void writeGPIOPin(GPIOPin * this, GPIOPinState state) {
    if (state == LOW) {
        GPIO_ResetBits(this->_port, this->_pin);
    } else {
        GPIO_SetBits(this->_port, this->_pin);
    }
}

PUBLIC void getGPIOPinPortSource(GPIOPin * this, uint8_t * portSource) {
    if (this->_port == GPIOA) {
        *portSource = GPIO_PortSourceGPIOA;
    } else if (this->_port == GPIOB) {
        *portSource = GPIO_PortSourceGPIOB;
    } else if (this->_port == GPIOC) {
        *portSource = GPIO_PortSourceGPIOC;
    } else if (this->_port == GPIOD) {
        *portSource = GPIO_PortSourceGPIOD;
    } else if (this->_port == GPIOE) {
        *portSource = GPIO_PortSourceGPIOE;
    } else if (this->_port == GPIOF) {
        *portSource = GPIO_PortSourceGPIOF;
    } else {
        if (this->_port == GPIOG) {
            *portSource = GPIO_PortSourceGPIOG;
        }
    }
}

PUBLIC void getGPIOPinPinSource(GPIOPin * this, uint8_t * pinSource) {
    *pinSource = 0;
    for (uint16_t pin = this->_pin; pin; *pinSource++, pin >>= 1);
}
