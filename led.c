#include "led.h"

PUBLIC LED newLED(GPIOPin pin, GPIOPinState lightState) {
    LED led = {
        ._pin = pin, 
        ._statLight = lightState
    };

    return led;
}

PUBLIC void lightUpLED(LED * this) {
    configGPIOPin(&this->_pin, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
    writeGPIOPin(&this->_pin, this->_statLight);
}

PUBLIC void blackOutLED(LED * this) {
    configGPIOPin(&this->_pin, GPIO_Speed_2MHz, GPIO_Mode_Out_OD);
    writeGPIOPin(&this->_pin, HIGH);
}
