#include "led.h"

PUBLIC LED newLED(GPIOPin pin, GPIOPinState lightState)
{
    LED led = {
        ._pin       = pin,
        ._statLight = lightState
    };

    setupGPIOPin(&led._pin, OUTPUT);

    return led;
}

PUBLIC void onLED(LED * pThis)
{
    writeGPIOPin(&pThis->_pin, pThis->_statLight);
}

PUBLIC void offLED(LED * pThis)
{
    writeGPIOPin(&pThis->_pin, (GPIOPinState)!pThis->_statLight);
}

PUBLIC STATIC void vTestLEDTask(void * pLED)
{
    for (;;) {
        onLED(pLED);
        vTaskDelay(1000);

        offLED(pLED);
        vTaskDelay(1000);
    }
}
