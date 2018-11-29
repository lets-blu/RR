#include "lampstate.h"
#include "lamp.h"

// virtual method implement(s)
PUBLIC VIRTUAL void initialTurnRedLamp(Lamp * lamp);
PUBLIC VIRTUAL void redTurnGreenLamp(Lamp * lamp);
PUBLIC VIRTUAL void greenTurnRedLamp(Lamp * lamp);

const struct ILampState LAMP_STATE_INITIAL = {
	.turnRed	= initialTurnRedLamp, 
	.turnGreen	= NULL
};

const struct ILampState LAMP_STATE_RED = {
    .turnRed    = NULL, 
    .turnGreen  = redTurnGreenLamp
};

const struct ILampState LAMP_STATE_YELLOW = {
    .turnRed    = NULL, 
    .turnGreen  = NULL
};

const struct ILampState LAMP_STATE_GREEN = {
    .turnRed    = greenTurnRedLamp,
    .turnGreen  = NULL
};

PUBLIC VIRTUAL void initialTurnRedLamp(Lamp * lamp) {
	resetRegisterGroupBit(lamp->_group, lamp->_yellow);
	setRegisterGroupBit(lamp->_group, lamp->_red);
	outputRegisterGroup(lamp->_group);
	
	setLampState(lamp, &LAMP_STATE_RED);
}

PUBLIC VIRTUAL void redTurnGreenLamp(Lamp * lamp) {
	resetRegisterGroupBit(lamp->_group, lamp->_red);
    setRegisterGroupBit(lamp->_group, lamp->_green);
    outputRegisterGroup(lamp->_group);

    setLampState(lamp, &LAMP_STATE_GREEN);
}

PUBLIC VIRTUAL void greenTurnRedLamp(Lamp * lamp) {
	// blink green
	__RELOAD_WATCHDOG();
	for (uint8_t i = 0; i < BLINKING_GREEN_LAMP_TIMES; i++) {
        resetRegisterGroupBit(lamp->_group, lamp->_green);
		outputRegisterGroup(lamp->_group);
        __BLINKING_GREEN_LAMP_DELAY();

        setRegisterGroupBit(lamp->_group, lamp->_green);
		outputRegisterGroup(lamp->_group);
        __BLINKING_GREEN_LAMP_DELAY();
    }

	// turn yellow
	__RELOAD_WATCHDOG();
	resetRegisterGroupBit(lamp->_group, lamp->_green);
	setRegisterGroupBit(lamp->_group, lamp->_yellow);
	outputRegisterGroup(lamp->_group);
	setLampState(lamp, &LAMP_STATE_YELLOW);
	__TURN_YELLOW_LAMP_DELAY();

	//turn red
	resetRegisterGroupBit(lamp->_group, lamp->_yellow);
	setRegisterGroupBit(lamp->_group, lamp->_red);
	outputRegisterGroup(lamp->_group);
	setLampState(lamp, &LAMP_STATE_RED);
}
