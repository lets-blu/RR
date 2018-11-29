#ifndef __MANUAL_H__
#define __MANUAL_H__

#include "FreeRTOS.h"
#include "stdbool.h"
#include "string.h"
#include "task.h"

#include "button.h"
#include "keyword.h"
#include "led.h"
#include "manualstate.h"
#include "watchdog.h"

#define NUM_OF_MANUAL_LEDS      ((uint8_t)5)
#define MANUAL_LEFT_TURN_LED    ((uint8_t)0)
#define MANUAL_STRAIGHT_LED     ((uint8_t)1)
#define MANUAL_ALL_RED_LED      ((uint8_t)2)
#define MANUAL_EAST_WEST_LED    ((uint8_t)3)
#define MANUAL_SOUTH_NORTH_LED  ((uint8_t)4)

#define NUM_OF_MANUAL_BUTTONS   ((uint8_t)4)
#define MANUAL_LAMP_BUTTON      ((uint8_t)0)
#define MANUAL_ROAD_BUTTON      ((uint8_t)1)
#define MANUAL_OK_BUTTON        ((uint8_t)2)
#define MANUAL_QUIT_BUTTON      ((uint8_t)3)

typedef struct Manual {
    const struct IManualState * _currentState;

    LED * _leds[NUM_OF_MANUAL_LEDS];
    Button * _buttons[NUM_OF_MANUAL_BUTTONS];
} Manual;

extern PUBLIC STATIC xTaskHandle xScanButtonsTask;
PUBLIC void staticManual(void);
PUBLIC Manual newManual(LED * leds[], Button * buttons[]);

PUBLIC void setManualState(Manual * this, const struct IManualState * newState);
PUBLIC const char * getManualStateName(Manual * this);

PUBLIC STATIC void vScanButtonsTask(void * pvArg);

#endif // __MANUAL_H__
