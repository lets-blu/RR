#ifndef __BUTTON_H__
#define __BUTTON_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdlib.h"
#include "stdbool.h"

#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "gpiopin.h"
#include "keywords.h"

#ifdef INCLUDE_vTaskDelay
#define BUTTON_DEBOUNCE() vTaskDelay(25 / portTICK_RATE_MS)
#else
#define BUTTON_DEBOUNCE()
#endif // INCLUDE_vTaskDelay

    typedef struct Button
    {
        GPIOPin _pin;
        GPIOPinState _statClick;

        xSemaphoreHandle _semaphore;

        void (*onClick)(struct Button *pThis);
    } Button;

    // (de)constructor(s)
    PUBLIC Button newButton(GPIOPin pin, GPIOPinState clickState);

    // public method(s)
    PUBLIC bool isButtonClicked(Button *pThis);
    PUBLIC bool isButtonInterruptEnable(Button *pThis);
    PUBLIC void setButtonInterrupt(Button *pThis, FunctionalState newState);
    PUBLIC VIRTUAL void defaultOnButtonClick(Button *pThis);

    PUBLIC STATIC void vButtonInterruptHandler(void *pButton);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __BUTTON_H__
