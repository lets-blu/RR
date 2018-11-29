#ifndef __LED_H__
#define __LED_H__

#include "stdbool.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"

#include "gpiopin.h"
#include "keyword.h"

#define DEFAULT_LEFTTURN_LED	(&led1)
#define DEFAULT_STRAIGHT_LED	(&led2)
#define DEFAULT_ALL_RED_LED		(&led3)
#define DEFAULT_EAST_WEST_LED	(&led4)
#define DEFAULT_SOUTH_NORTH_LED	(&led5)

typedef struct LED {
    GPIOPin _pin;
    GPIOPinState _statLight;
} LED;

extern STATIC LED led1, led2, led3, led4, led5;
PUBLIC void staticLED(void);

PUBLIC void lightUpLED(LED * this);
PUBLIC void blackOutLED(LED * this);

#endif // __LED_H__
