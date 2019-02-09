#include "led.h"

PUBLIC LED newLED(GPIOPin pin, GPIOPinState lightState) {
    LED led = {
        ._pin = pin,
        ._statLight = lightState
    };

    return led;
}

PUBLIC void lightUpLED(LED * pThis) {
    configGPIOPin(&pThis->_pin, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
    writeGPIOPin(&pThis->_pin, pThis->_statLight);
}

PUBLIC void blackOutLED(LED * pThis) {
    configGPIOPin(&pThis->_pin, GPIO_Speed_2MHz, GPIO_Mode_Out_OD);
    writeGPIOPin(&pThis->_pin, HIGH);
}
