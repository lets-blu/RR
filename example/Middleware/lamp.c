#include "lamp.h"

// private method(s)
PRIVATE void initLamp(Lamp * this);

PUBLIC void staticLamp(void) {
	static bool called = false;
	
    if (called) {
        return;
    }

    staticRegisterGroup();

	called = true;
}

PUBLIC Lamp newLamp(RegisterGroup * group, 
        uint8_t red, uint8_t yellow, uint8_t green) {
    Lamp lamp = {
        ._currentState  = &LAMP_STATE_INITIAL       , 
        ._group         = group                     , 
        ._red           = red                       , 
        ._yellow        = yellow                    , 
        ._green         = green                     , 
        ._semphr        = xSemaphoreCreateMutex()
    };
	
	initLamp(&lamp);

    return lamp;
}

PUBLIC void turnRedLamp(Lamp * this) {
    if (this->_currentState->turnRed != NULL) {
        this->_currentState->turnRed(this);
    }
}

PUBLIC void turnGreenLamp(Lamp * this) {
    if (this->_currentState->turnGreen != NULL) {
        this->_currentState->turnGreen(this);
    }
}

PUBLIC void setLampState(Lamp * this, const struct ILampState * newState) {
    this->_currentState = newState;
}

PRIVATE void initLamp(Lamp * this) {
	resetRegisterGroupBit(this->_group, this->_green);
	resetRegisterGroupBit(this->_group, this->_red);
    setRegisterGroupBit(this->_group, this->_yellow);
    
	outputRegisterGroup(this->_group);
}
