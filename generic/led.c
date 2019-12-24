#include "led.h"

PUBLIC LED newLED(GPIOPin pin, GPIOPinState lightState)
{
    LED led = {
        ._pin           = pin, 
        ._lightState    = lightState
    };

    setupGPIOPin(&pin, OUTPUT);
    turnOffLED(&led);

    return led;
}

PUBLIC void turnOnLED(LED * pThis)
{
    writeGPIOPin(&pThis->_pin, pThis->_lightState);
}

PUBLIC void turnOffLED(LED * pThis)
{
    writeGPIOPin(&pThis->_pin, (GPIOPinState)(!pThis->_lightState));
}
