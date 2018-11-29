#include "led.h"

// (de)constructor(s)
PRIVATE LED newLED(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin_x, 
        GPIOPinState lightState);

// static member(s)
PUBLIC STATIC LED led1, led2, led3, led4, led5;

PUBLIC void staticLED(void) {
    static bool called = false;

    if (called) {
        return;
    }

    led1 = newLED(GPIOB, GPIO_Pin_4 , LOW);
    led2 = newLED(GPIOA, GPIO_Pin_11, LOW);
    led3 = newLED(GPIOA, GPIO_Pin_10, LOW);
    led4 = newLED(GPIOA, GPIO_Pin_8 , LOW);
    led5 = newLED(GPIOA, GPIO_Pin_9 , LOW);

    called = true;
}

PRIVATE LED newLED(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin_x, 
        GPIOPinState lightState) {
    LED led = {
        ._pin       = newGPIOPin(GPIOx, GPIO_Pin_x) , 
        ._statLight = lightState
    };

    // // use PC13/14/15 as GPIO pin
    // // reference: http://blog.gkong.com/hushunlin_219189.ashx
    // if (GPIOx == GPIOC && (GPIO_Pin_x & 0xe000)) {
    //     RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    //     PWR_BackupAccessCmd(ENABLE);
    //     RCC_LSEConfig(RCC_LSE_OFF);
    // }
    
    if (GPIOx == GPIOB && GPIO_Pin_x == GPIO_Pin_4) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    }
    
    return led;
}

PUBLIC void lightUpLED(LED * this) {
    configGPIOPin(&this->_pin, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
    
    if (this->_statLight == LOW) {
        GPIO_ResetBits(this->_pin._port, this->_pin._pin);
    } else {
        GPIO_SetBits(this->_pin._port, this->_pin._pin);
    }
}

PUBLIC void blackOutLED(LED * this) {
    configGPIOPin(&this->_pin, GPIO_Speed_2MHz, GPIO_Mode_Out_OD);
    GPIO_SetBits(this->_pin._port, this->_pin._pin);
}
