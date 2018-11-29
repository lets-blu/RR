#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "FreeRTOS.h"
#include "semphr.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stm32f10x_exti.h"
#include "task.h"

#include "gpiopin.h"
#include "keyword.h"

#define DEFAULT_LAMP_BUTTON	(&button1)
#define DEFAULT_ROAD_BUTTON	(&button2)
#define DEFAULT_OK_BUTTON	(&button3)
#define DEFAULT_QUIT_BUTTON	(&button4)

#define __BUTTON_DEBOUNCE() vTaskDelay(25 / portTICK_RATE_MS)

typedef struct Button {
    GPIOPin _pin;
    GPIOPinState _statClick;
    FunctionalState _intEnable;
    
    void (*onClick)(struct Button * this, void * args);
} Button;

extern PUBLIC STATIC Button button1, button2, button3, button4;
PUBLIC void staticButton(void);

PUBLIC bool isButtonClicked(Button * this);

#endif // __BUTTON_H__
