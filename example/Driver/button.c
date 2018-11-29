#include "button.h"

#include "controller.h"

// (de)constructor(s)
PRIVATE Button newButton(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin_x, 
        GPIOPinState clickState);

// private method(s)
PRIVATE void setButtonInterrupt(Button * this, 
        uint8_t preemptionPriority, uint8_t subPriority, 
        FunctionalState newState);
PRIVATE void getButtonPortSource(Button * this, uint8_t * portSource);
PRIVATE void getButtonPinSource(Button * this, uint8_t * pinSource);
PRIVATE void getButtonEXTILine(Button * this, uint32_t * extiLine);
PRIVATE void getButtonIRQChannel(Button * this, uint8_t * irqChannel);

// static member(s)
PUBLIC STATIC Button button1, button2, button3, button4;
PRIVATE STATIC xSemaphoreHandle xEXTI3HandleSemaphore = NULL;

// private static method(s)
PRIVATE STATIC void vEXTI3HandleTask(void * pvArg);

PUBLIC void staticButton() {
    static bool called = false;

    if (called) {
        return;
    }

    button1 = newButton(GPIOB, GPIO_Pin_1, LOW);
    button2 = newButton(GPIOB, GPIO_Pin_0, LOW);
    button3 = newButton(GPIOA, GPIO_Pin_3, LOW);
    button4 = newButton(GPIOA, GPIO_Pin_2, LOW);

    setButtonInterrupt(&button3, 0, 0, ENABLE);
	vSemaphoreCreateBinary(xEXTI3HandleSemaphore);
	xTaskCreate(vEXTI3HandleTask, "EXTI3Task", configMINIMAL_STACK_SIZE, NULL, 
        2, NULL);
    
    called = true;
}

PRIVATE Button newButton(GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin_x, 
        GPIOPinState clickState) {
    Button button = {
        ._pin       = newGPIOPin(GPIOx, GPIO_Pin_x) , 
        ._statClick = clickState                    , 
        ._intEnable = DISABLE                       , 
        .onClick    = NULL
    };

    // // disable JTAG to use PB3 as GPIO pin
    // // reference: http://www.eeworld.com.cn/mcu/article_2016060226708.html
    // if (GPIOx == GPIOB && GPIO_Pin_x == GPIO_Pin_3) {
    //     RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    //     GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    // }

    configGPIOPin(&button._pin, GPIO_Speed_2MHz, GPIO_Mode_IPU);
    
    return button;
}

PUBLIC bool isButtonClicked(Button * this) {
	__BUTTON_DEBOUNCE();
    uint8_t state = GPIO_ReadInputDataBit(this->_pin._port, this->_pin._pin);
    
    return state == this->_statClick;
}

PRIVATE void setButtonInterrupt(Button * this, 
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

PRIVATE STATIC void vEXTI3HandleTask(void * pvArg) {
	for (;;) {
		xSemaphoreTake(xEXTI3HandleSemaphore, portMAX_DELAY);
		
		if (isButtonClicked(&button3)) {
            button3.onClick(&button3, &controller->_manual);
			__BUTTON_DEBOUNCE();
		}
	}
}

void EXTI3_IRQHandler(void) {
	static portBASE_TYPE xResult = pdFALSE;
	
	if (EXTI_GetITStatus(EXTI_Line3) == SET) {
		EXTI_ClearITPendingBit(EXTI_Line3);
		xSemaphoreGiveFromISR(xEXTI3HandleSemaphore, &xResult);
	}
}
