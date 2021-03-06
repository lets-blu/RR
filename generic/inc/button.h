#ifndef __BUTTON_H__
#define __BUTTON_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

#include "ut.h"
#include "gpiopin.h"
#include "keywords.h"

#ifdef UNIT_TEST
#include "ut_cmsis_os.h"
#include "ut_hal.h"
#include "ut_hal_cortex.h"
#else
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_cortex.h"
#endif // UNIT_TEST

typedef struct Button {
    GPIOPin _pin;
    GPIOPinState _clickState;

    osThreadId _interruptHandler;
    osSemaphoreId _interruptSemaphore;

    void (*onClick)(struct Button * pThis);
} Button;

typedef void (*on_button_click_fp)(Button *);

// Constructor
PUBLIC Button newButton(GPIOPin pin, GPIOPinState clickState);

// Public method(s)
PUBLIC bool isButtonClicked(Button * pThis);
PUBLIC void onButtonInterruptOccurred(Button * pThis);

PUBLIC bool isButtonInterruptEnabled(Button * pThis);
PUBLIC void setButtonInterruptEnabled(Button * pThis, bool enabled);

PUBLIC VIRTUAL void defaultOnButtonClick(Button * pThis);
PUBLIC STATIC void vButtonInterruptHandler(void const * argument);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __BUTTON_H__
