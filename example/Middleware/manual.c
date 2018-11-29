#include "manual.h"

// virtual method implement(s)
PUBLIC VIRTUAL void manualOnLampButtonClick(Button * button, Manual * manual);
PUBLIC VIRTUAL void manualOnRoadButtonClick(Button * button, Manual * manual);
PUBLIC VIRTUAL void manualOnOKButtonClick(Button * button, Manual * manual);
PUBLIC VIRTUAL void manualOnQuitButtonClick(Button * button, Manual * manual);

// static member(s)
PUBLIC STATIC xTaskHandle xScanButtonsTask = NULL;

PUBLIC void staticManual(void) {
	static bool called = false;

    if (called) {
        return;
    }

    staticButton();
	staticLED();

	called = true;
}

PUBLIC Manual newManual(LED * leds[NUM_OF_MANUAL_LEDS], 
        Button * buttons[NUM_OF_MANUAL_BUTTONS]) {
    Manual manual = {
        ._currentState = &MANUAL_STATE_INITIAL
    };

    memcpy(manual._leds, leds, NUM_OF_MANUAL_LEDS * sizeof(LED *));
    memcpy(manual._buttons, buttons, NUM_OF_MANUAL_BUTTONS * sizeof(Button *));
    
    manual._buttons[MANUAL_LAMP_BUTTON]->onClick
            = (void (*)(Button *, void *))manualOnLampButtonClick;
    manual._buttons[MANUAL_ROAD_BUTTON]->onClick
            = (void (*)(Button *, void *))manualOnRoadButtonClick;
    manual._buttons[MANUAL_OK_BUTTON]->onClick
            = (void (*)(Button *, void *))manualOnOKButtonClick;
    manual._buttons[MANUAL_QUIT_BUTTON]->onClick
            = (void (*)(Button *, void *))manualOnQuitButtonClick;

    return manual;
}

PUBLIC void setManualState(Manual * this, const struct IManualState * newState) {
    this->_currentState = newState;
}

PUBLIC const char * getManualStateName(Manual * this) {
    if (this->_currentState->getName != NULL) {
        return this->_currentState->getName(this);
    }

    return NULL;
}

PUBLIC VIRTUAL void manualOnLampButtonClick(Button * button, Manual * manual) {
    if (manual->_currentState->onLampButtonClick != NULL) {
        manual->_currentState->onLampButtonClick(manual);
    }
}

PUBLIC VIRTUAL void manualOnRoadButtonClick(Button * button, Manual * manual) {
    if (manual->_currentState->onRoadButtonClick != NULL) {
        manual->_currentState->onRoadButtonClick(manual);
    }
}

PUBLIC VIRTUAL void manualOnOKButtonClick(Button * button, Manual * manual) {
    if (manual->_currentState->onOKButtonClick != NULL) {
        manual->_currentState->onOKButtonClick(manual);
    }
}

PUBLIC VIRTUAL void manualOnQuitButtonClick(Button * button, Manual * manual) {
    if (manual->_currentState->onQuitButtonClick != NULL) {
        manual->_currentState->onQuitButtonClick(manual);
    }
}

void vScanButtonsTask(void * pvArg) {
	Manual * manual = pvArg;
	
	for (;;) {
        FOREACH (Button *, button, manual->_buttons) {
            __RELOAD_WATCHDOG();
            // precision delay for improved button experience
            portTickType xLastWeakTime = xTaskGetTickCount();
            
            if (button->_intEnable == DISABLE && isButtonClicked(button)) {
                if (button->onClick != NULL) {
                    button->onClick(button, manual);
                }

                __BUTTON_DEBOUNCE();
            }
            
            vTaskDelayUntil(&xLastWeakTime, 50 / portTICK_RATE_MS);
        }
	}
}
