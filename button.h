#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "stdbool.h"
#include "stdlib.h"
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "gpiopin.h"
#include "keywords.h"

#ifdef INCLUDE_vTaskDelay
#define __BUTTON_DEBOUNCE() vTaskDelay(25 / portTICK_RATE_MS)
#else
#define __BUTTON_DEBOUNCE() 
#endif // INCLUDE_vTaskDelay

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct Button {
    GPIOPin _pin;
    GPIOPinState _statClick;
    FunctionalState _intEnable;
    
    void (*onClick)(struct Button * this, void * args);
} Button;

// (de)constructor(s)
PUBLIC Button newButton(GPIOPin pin, GPIOPinState clickState);

// public method(s)
PUBLIC bool isButtonClicked(Button * this);
PUBLIC void setButtonInterrupt(Button * this, 
        uint8_t preemptionPriority, uint8_t subPriority, 
        FunctionalState newState);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __BUTTON_H__
