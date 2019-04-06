#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "FreeRTOS.h"
#include "task.h"

#include "gpiopin.h"
#include "keywords.h"

    typedef struct LED
    {
        GPIOPin _pin;
        GPIOPinState _statLight;
    } LED;

    // (de)constructor(s)
    PUBLIC LED newLED(GPIOPin pin, GPIOPinState lightState);

    // public method(s)
    PUBLIC void turnOnLED(LED *pThis);
    PUBLIC void turnOffLED(LED *pThis);

    PUBLIC STATIC void vTestLEDTask(void *pLED);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LED_H__
