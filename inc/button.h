#ifndef __BUTTON_H__
#define __BUTTON_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"

#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_cortex.h"

#include "gpiopin.h"
#include "keywords.h"

typedef struct Button
{
    GPIOPin _pin;
    GPIOPinState _clickState;

    osThreadId _interruptHandler;
    osSemaphoreId _interruptSemaphore;

    void (*onClick)(struct Button * pThis);
} Button;

// constructor
PUBLIC Button newButton(GPIOPin pin, GPIOPinState clickState);

// public method(s)
PUBLIC bool isButtonClicked(Button * pThis);
PUBLIC bool isButtonInterruptEnabled(Button * pThis);
PUBLIC void setButtonInterrupt(Button * pThis, FunctionalState state);

PUBLIC VIRTUAL void defaultOnButtonClick(Button * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __BUTTON_H__
