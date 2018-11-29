#include "button.h"

// private method(s)
PRIVATE void getButtonPortSource(Button * this, uint8_t * portSource);
PRIVATE void getButtonPinSource(Button * this, uint8_t * pinSource);
PRIVATE void getButtonEXTILine(Button * this, uint32_t * extiLine);
PRIVATE void getButtonIRQChannel(Button * this, uint8_t * irqChannel);

PUBLIC Button newButton(GPIOPin pin, GPIOPinState clickState) {
    Button button = {
        ._pin = pin, 
        ._statClick = clickState, 
        ._intEnable = DISABLE, 
        .onClick = NULL
    };

    configGPIOPin(&button._pin, GPIO_Speed_2MHz, GPIO_Mode_IPU);

    return button;
}

PUBLIC bool isButtonClicked(Button * this) {
    __BUTTON_DEBOUNCE();
    return readGPIOPin(&this->_pin) == this->_statClick;
}

PUBLIC void setButtonInterrupt(Button * this, 
        uint8_t preemptionPriority, uint8_t subPriority, 
        FunctionalState newState) {
    // AFIO is used by other place, never disable it.
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    uint8_t portSource, pinSource;
    getButtonPortSource(this, &portSource);
    getButtonPinSource(this, &pinSource);
    GPIO_EXTILineConfig(portSource, pinSource);
    
    uint32_t extiLine;
    getButtonEXTILine(this, &extiLine);
    EXTI_InitTypeDef EXTI_InitStructure = {
        .EXTI_Line      = extiLine              , 
        .EXTI_Mode      = EXTI_Mode_Interrupt   , 
        .EXTI_Trigger   = EXTI_Trigger_Falling  , 
        .EXTI_LineCmd   = newState
    };
    
    EXTI_Init(&EXTI_InitStructure);

    uint8_t irqChannel;
    getButtonIRQChannel(this, &irqChannel);
    NVIC_InitTypeDef NVIC_InitStructure = {
        .NVIC_IRQChannel                    = irqChannel        , 
        .NVIC_IRQChannelPreemptionPriority  = preemptionPriority, 
        .NVIC_IRQChannelSubPriority         = subPriority       , 
        .NVIC_IRQChannelCmd                 = newState
    };
    
    NVIC_Init(&NVIC_InitStructure);

    this->_intEnable = newState;
}

PRIVATE void getButtonPortSource(Button * this, uint8_t * portSource) {
    if (this->_pin._port == GPIOA) {
        *portSource = GPIO_PortSourceGPIOA;
    } else if (this->_pin._port == GPIOB) {
        *portSource = GPIO_PortSourceGPIOB;
    } else if (this->_pin._port == GPIOC) {
        *portSource = GPIO_PortSourceGPIOC;
    } else if (this->_pin._port == GPIOD) {
        *portSource = GPIO_PortSourceGPIOD;
    } else if (this->_pin._port == GPIOE) {
        *portSource = GPIO_PortSourceGPIOE;
    } else if (this->_pin._port == GPIOF) {
        *portSource = GPIO_PortSourceGPIOF;
    } else {
        if (this->_pin._port == GPIOG) {
            *portSource = GPIO_PortSourceGPIOG;
        }
    }
}

PRIVATE void getButtonPinSource(Button * this, uint8_t * pinSource) {
    *pinSource = 0;
    for (uint16_t pin = this->_pin._pin; pin; *pinSource++, pin >>= 1);
}

PRIVATE void getButtonEXTILine(Button * this, uint32_t * extiLine) {
    *extiLine = this->_pin._pin;
}

PRIVATE void getButtonIRQChannel(Button * this, uint8_t * irqChannel) {
    switch (this->_pin._pin) {
        case GPIO_Pin_0:
            *irqChannel = EXTI0_IRQn;
            break;

        case GPIO_Pin_1:
            *irqChannel = EXTI1_IRQn;
            break;

        case GPIO_Pin_2:
            *irqChannel = EXTI2_IRQn;
            break;

        case GPIO_Pin_3:
            *irqChannel = EXTI3_IRQn;
            break;

        case GPIO_Pin_4:
            *irqChannel = EXTI4_IRQn;
            break;

        case GPIO_Pin_5:
        case GPIO_Pin_6:
        case GPIO_Pin_7:
        case GPIO_Pin_8:
        case GPIO_Pin_9:
            *irqChannel = EXTI9_5_IRQn;
            break;
            
        case GPIO_Pin_10:
        case GPIO_Pin_11:
        case GPIO_Pin_12:
        case GPIO_Pin_13:
        case GPIO_Pin_14:
        case GPIO_Pin_15:
            *irqChannel = EXTI15_10_IRQn;
            break;
    }
}
