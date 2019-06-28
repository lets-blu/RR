#include "led.h"

PUBLIC LED newLED(GPIOPin pin, GPIOPinState lightState)
{
    LED led = 
    {
        ._pin       = pin, 
        ._statLight = lightState
    };

    setupGPIOPin(&led._pin, OUTPUT);
    turnOffLED(&led);

    return led;
}

PUBLIC void turnOnLED(LED * pThis)
{
    writeGPIOPin(&pThis->_pin, pThis->_statLight);
}

PUBLIC void turnOffLED(LED * pThis)
{
    writeGPIOPin(&pThis->_pin, (GPIOPinState)!pThis->_statLight);
}
