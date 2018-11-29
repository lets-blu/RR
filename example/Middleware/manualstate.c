#include "manualstate.h"
#include "manual.h"

#include "controller.h"

// virtual method implement(s)
PUBLIC VIRTUAL void onOKButtonClick(Manual * manual);
PUBLIC VIRTUAL void onQuitButtonClick(Manual * manual);
PUBLIC VIRTUAL void onInitialEnterButtonClick(Manual * manual);
PUBLIC VIRTUAL const char * getAllRedName(Manual * manual);
PUBLIC VIRTUAL void onAllRedLampButtonClick(Manual * manual);
PUBLIC VIRTUAL void onAllRedRoadButtonClick(Manual * manual);
PUBLIC VIRTUAL const char * getEastWestLeftTurnName(Manual * manual);
PUBLIC VIRTUAL void onEastWestLeftTurnLampButtonClick(Manual * manual);
PUBLIC VIRTUAL void onEastWestLeftTurnRoadButtonClick(Manual * manual);
PUBLIC VIRTUAL const char * getEastWestStraightName(Manual * manual);
PUBLIC VIRTUAL void onEastWestStraightLampButtonClick(Manual * manual);
PUBLIC VIRTUAL void onEastWestStraightRoadButtonClick(Manual * manual);
PUBLIC VIRTUAL const char * getSouthNorthLeftTurnName(Manual * manual);
PUBLIC VIRTUAL void onSouthNorthLeftTurnLampButtonClick(Manual * manual);
PUBLIC VIRTUAL void onSouthNorthLeftTurnRoadButtonClick(Manual * manual);
PUBLIC VIRTUAL const char * getSouthNorthStraightName(Manual * manual);
PUBLIC VIRTUAL void onSouthNorthStraightLampButtonClick(Manual * manual);
PUBLIC VIRTUAL void onSouthNorthStraightRoadButtonClick(Manual * manual);

const struct IManualState MANUAL_STATE_INITIAL = {
	.getName			= NULL, 
    .onLampButtonClick  = NULL, 
    .onRoadButtonClick  = NULL, 
    .onOKButtonClick    = onInitialEnterButtonClick, 
    .onQuitButtonClick  = NULL
};

const struct IManualState MANUAL_STATE_ALL_RED = {
	.getName			= getAllRedName, 
    .onLampButtonClick  = onAllRedLampButtonClick, 
    .onRoadButtonClick  = onAllRedRoadButtonClick, 
    .onOKButtonClick    = onOKButtonClick, 
    .onQuitButtonClick  = onQuitButtonClick
};

const struct IManualState MANUAL_STATE_EASTWEST_LEFTTURN = {
	.getName			= getEastWestLeftTurnName, 
    .onLampButtonClick  = onEastWestLeftTurnLampButtonClick, 
    .onRoadButtonClick  = onEastWestLeftTurnRoadButtonClick, 
    .onOKButtonClick    = onOKButtonClick, 
    .onQuitButtonClick  = onQuitButtonClick
};

const struct IManualState MANUAL_STATE_EASTWEST_STRAIGHT = {
	.getName			= getEastWestStraightName, 
    .onLampButtonClick  = onEastWestStraightLampButtonClick, 
    .onRoadButtonClick  = onEastWestStraightRoadButtonClick, 
    .onOKButtonClick    = onOKButtonClick, 
    .onQuitButtonClick  = onQuitButtonClick
};

const struct IManualState MANUAL_STATE_SOUTHNORTH_LEFTTURN = {
	.getName			= getSouthNorthLeftTurnName, 
    .onLampButtonClick  = onSouthNorthLeftTurnLampButtonClick, 
    .onRoadButtonClick  = onSouthNorthLeftTurnRoadButtonClick, 
    .onOKButtonClick    = onOKButtonClick, 
    .onQuitButtonClick  = onQuitButtonClick
};

const struct IManualState MANUAL_STATE_SOUTHNORTH_STRAIGHT = {
	.getName			= getSouthNorthStraightName, 
    .onLampButtonClick  = onSouthNorthStraightLampButtonClick, 
    .onRoadButtonClick  = onSouthNorthStraightRoadButtonClick, 
    .onOKButtonClick    = onOKButtonClick, 
    .onQuitButtonClick  = onQuitButtonClick
};

PUBLIC VIRTUAL void onOKButtonClick(Manual * manual) {
    const char * name = NULL;
    
    if ((name = getManualStateName(manual)) != NULL) {
        xQueueSendToBack(controller->_xManualQueue, &name, 0);
    }
}

PUBLIC VIRTUAL void onQuitButtonClick(Manual * manual) {
    FOREACH(LED *, led, manual->_leds) {
        blackOutLED(led);
    }
    
    const char * name = QUIT_STRING;
	xQueueSendToBack(controller->_xManualQueue, &name, 0);
    
    setManualState(manual, &MANUAL_STATE_INITIAL);
    vTaskDelete(NULL);
}

PUBLIC VIRTUAL void onInitialEnterButtonClick(Manual * manual) {
    lightUpLED(manual->_leds[MANUAL_ALL_RED_LED]);
	
    setManualState(manual, &MANUAL_STATE_ALL_RED);
	xTaskCreate(vScanButtonsTask, "ScanButtonsTask", configMINIMAL_STACK_SIZE, 
            manual, 2, &xScanButtonsTask);
}

PUBLIC VIRTUAL const char * getAllRedName(Manual * manual) {
	return ALLRED_STRING;
}

PUBLIC VIRTUAL void onAllRedLampButtonClick(Manual * manual) {
    blackOutLED(manual->_leds[MANUAL_ALL_RED_LED]);
    lightUpLED(manual->_leds[MANUAL_LEFT_TURN_LED]);
    lightUpLED(manual->_leds[MANUAL_EAST_WEST_LED]);
    
    setManualState(manual, &MANUAL_STATE_EASTWEST_LEFTTURN);
}

PUBLIC VIRTUAL void onAllRedRoadButtonClick(Manual * manual) {
    blackOutLED(manual->_leds[MANUAL_ALL_RED_LED]);
    lightUpLED(manual->_leds[MANUAL_LEFT_TURN_LED]);
    lightUpLED(manual->_leds[MANUAL_EAST_WEST_LED]);
    
    setManualState(manual, &MANUAL_STATE_EASTWEST_LEFTTURN);
}

PUBLIC VIRTUAL const char * getEastWestLeftTurnName(Manual * manual) {
	return EASTWEST_LEFTTURN_STRING;
}

PUBLIC VIRTUAL void onEastWestLeftTurnLampButtonClick(Manual * manual) {
    blackOutLED(manual->_leds[MANUAL_LEFT_TURN_LED]);
    lightUpLED(manual->_leds[MANUAL_STRAIGHT_LED]);
    
    setManualState(manual, &MANUAL_STATE_EASTWEST_STRAIGHT);
}

PUBLIC VIRTUAL void onEastWestLeftTurnRoadButtonClick(Manual * manual) {
    blackOutLED(manual->_leds[MANUAL_EAST_WEST_LED]);
    lightUpLED(manual->_leds[MANUAL_SOUTH_NORTH_LED]);
    
    setManualState(manual, &MANUAL_STATE_SOUTHNORTH_LEFTTURN);
}

PUBLIC VIRTUAL const char * getEastWestStraightName(Manual * manual) {
	return EASTWEST_STRAIGHT_STRING;
}

PUBLIC VIRTUAL void onEastWestStraightLampButtonClick(Manual * manual) {
    blackOutLED(manual->_leds[MANUAL_STRAIGHT_LED]);
    blackOutLED(manual->_leds[MANUAL_EAST_WEST_LED]);
    lightUpLED(manual->_leds[MANUAL_ALL_RED_LED]);

    setManualState(manual, &MANUAL_STATE_ALL_RED);
}

PUBLIC VIRTUAL void onEastWestStraightRoadButtonClick(Manual * manual) {
    blackOutLED(manual->_leds[MANUAL_EAST_WEST_LED]);
    lightUpLED(manual->_leds[MANUAL_SOUTH_NORTH_LED]);
    
    setManualState(manual, &MANUAL_STATE_SOUTHNORTH_STRAIGHT);
}

PUBLIC VIRTUAL const char * getSouthNorthLeftTurnName(Manual * manual) {
	return SOUTHNORTH_LEFTTURN_STRING;
}

PUBLIC VIRTUAL void onSouthNorthLeftTurnLampButtonClick(Manual * manual) {
    blackOutLED(manual->_leds[MANUAL_LEFT_TURN_LED]);
    lightUpLED(manual->_leds[MANUAL_STRAIGHT_LED]);
    
    setManualState(manual, &MANUAL_STATE_SOUTHNORTH_STRAIGHT);
}

PUBLIC VIRTUAL void onSouthNorthLeftTurnRoadButtonClick(Manual * manual) {
    blackOutLED(manual->_leds[MANUAL_SOUTH_NORTH_LED]);
    lightUpLED(manual->_leds[MANUAL_EAST_WEST_LED]);

    setManualState(manual, &MANUAL_STATE_EASTWEST_LEFTTURN);
}

PUBLIC VIRTUAL const char * getSouthNorthStraightName(Manual * manual) {
	return SOUTHNORTH_STRAIGHT_STRING;
}

 PUBLIC VIRTUAL void onSouthNorthStraightLampButtonClick(Manual * manual) {
    blackOutLED(manual->_leds[MANUAL_STRAIGHT_LED]);
    blackOutLED(manual->_leds[MANUAL_SOUTH_NORTH_LED]);
    lightUpLED(manual->_leds[MANUAL_ALL_RED_LED]);

    setManualState(manual, &MANUAL_STATE_ALL_RED);
}

PUBLIC VIRTUAL void onSouthNorthStraightRoadButtonClick(Manual * manual) {
    blackOutLED(manual->_leds[MANUAL_SOUTH_NORTH_LED]);
    lightUpLED(manual->_leds[MANUAL_EAST_WEST_LED]);

    setManualState(manual, &MANUAL_STATE_EASTWEST_STRAIGHT);
}
