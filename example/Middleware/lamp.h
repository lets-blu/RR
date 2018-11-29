#ifndef __LAMP_H__
#define __LAMP_H__

#include "FreeRTOS.h"
#include "semphr.h"
#include "stdbool.h"
#include "task.h"

#include "lampstate.h"
#include "registergroup.h"

#define NUM_OF_LAMP_LAMPS           ((uint8_t)3)
#define LAMP_RED_LOCATION           ((uint8_t)0)
#define LAMP_YELLOW_LOCATION        ((uint8_t)1)
#define LAMP_GREEN_LOCATION         ((uint8_t)2)

#define STARTUP_YELLOW_LAMP_TIME    ((uint8_t)3)
#define STARTUP_RED_LAMP_TIME       ((uint8_t)5)
#define DEFAULT_YELLOW_LAMP_TIME    ((uint8_t)3)
#define DEFAULT_GREEN_LAMP_TIME     ((uint8_t)10)
#define MINIMAL_GREEN_LAMP_TIME     ((uint8_t)5)
#define MAXIMAL_GREEN_LAMP_TIME     ((uint8_t)20)
#define BLINKING_GREEN_LAMP_TIMES   ((uint8_t)3)

#define __STARTUP_YELLOW_LAMP_DELAY() {                                                 \
    vTaskDelay(STARTUP_YELLOW_LAMP_TIME * 1000 / portTICK_RATE_MS);                     \
}

#define __STARTUP_RED_LAMP_DELAY() {                                                    \
    vTaskDelay(STARTUP_RED_LAMP_TIME * 1000 / portTICK_RATE_MS);                        \
}

#define __BLINKING_GREEN_LAMP_DELAY()   {                                               \
    portTickType xWeakTime = xTaskGetTickCount();                                       \
    vTaskDelayUntil(&xWeakTime, 500 / portTICK_RATE_MS);                                \
}

#define __TURN_YELLOW_LAMP_DELAY()      {                                               \
    portTickType xWeakTime = xTaskGetTickCount();                                       \
    vTaskDelayUntil(&xWeakTime, DEFAULT_YELLOW_LAMP_TIME * 1000 / portTICK_RATE_MS);    \
}

typedef struct Lamp {
    const struct ILampState * _currentState;

    RegisterGroup * _group;
    uint8_t _red, _yellow, _green;

    xSemaphoreHandle _semphr;
} Lamp;

PUBLIC void staticLamp(void);
PUBLIC Lamp newLamp(RegisterGroup * group, 
        uint8_t red, uint8_t yellow, uint8_t green);

PUBLIC void turnRedLamp(Lamp * this);
PUBLIC void turnGreenLamp(Lamp * this);
PUBLIC void setLampState(Lamp * this, const struct ILampState * newState);

#endif // __LAMP_H__
