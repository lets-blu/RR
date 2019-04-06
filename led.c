#include "led.h"

PUBLIC LED newLED(GPIOPin pin, GPIOPinState lightState)
{
    LED led = {
        ._pin = pin,
        ._statLight = lightState};

    setupGPIOPin(&led._pin, OUTPUT);

    return led;
}

PUBLIC void turnOnLED(LED *pThis)
{
    writeGPIOPin(&pThis->_pin, pThis->_statLight);
}

PUBLIC void turnOffLED(LED *pThis)
{
    writeGPIOPin(&pThis->_pin, (GPIOPinState)!pThis->_statLight);
}

PUBLIC STATIC void vTestLEDTask(void *pLED)
{
    for (;;)
    {
        turnOnLED(pLED);
        vTaskDelay(1000);

        turnOffLED(pLED);
        vTaskDelay(1000);
    }
}
