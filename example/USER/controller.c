#include "controller.h"

#define NONEXISTENT_DIRECTION ((int8_t)-1)

// (de)constructor
PRIVATE Controller * newController(uint8_t totalOfDirections);

// private method
PRIVATE uint8_t getControllerNextDirection(Controller * this);

// static member
PUBLIC STATIC Controller * controller = NULL;
PUBLIC STATIC xTaskHandle xAutoRunTask = NULL;
PUBLIC STATIC xSemaphoreHandle xAutoRunSemaphore = NULL;

// private static method
PRIVATE STATIC void sendControllerAutoRunMessage(uint8_t dischargeTime);
PRIVATE STATIC void sendControllerManualMessage(uint8_t roadIndex, uint8_t laneIndex);
PRIVATE STATIC void quitControllerManualMode(void);
PRIVATE STATIC void quitControllerAutoRunMode(void);
PRIVATE STATIC void handleControllerDischargeDirection(Direction * direction, bool isNext);

PUBLIC void staticController(void) {
	if (controller != NULL) {
		return;
	}

	staticManual();
	staticDirection();

	xAutoRunSemaphore = xSemaphoreCreateMutex();
	xSemaphoreGive(xAutoRunSemaphore);

	controller = newController(8);
}

PRIVATE Controller * newController(uint8_t totalOfDirections) {
	Controller * controller = malloc(
		sizeof(Controller) + totalOfDirections * sizeof(Direction *));
	
	if (controller != NULL) {
		controller->_xManualQueue = xQueueCreate(8, sizeof(const char *));
		if (controller->_xManualQueue == NULL) {
			free(controller);
			
			return NULL;
		}

#ifdef DEBUG
        vQueueAddToRegistry(controller->_xManualQueue, "ManualQueue");
#endif
		
		controller->_totalOfDirections = totalOfDirections;
		controller->_numOfDirections = 0;
		controller->_dirCurrent = 0;
		controller->_dirNext = 0;
	}
	
	return controller;
}

PUBLIC void deleteController(Controller * this) {
    free(this);
}

PUBLIC void setControllerManual(Controller * this, Manual manual) {
	this->_manual = manual;
}

PUBLIC void setControllerEthernet(Controller * this, struct Ethernet * ethernet) {
    this->_ethernet = ethernet;
}

PUBLIC void addDirectionToController(Controller * this, Direction * direction) {
	if (this->_numOfDirections < this->_totalOfDirections) {
		this->_directions[this->_numOfDirections++] = direction;
	}
}

PRIVATE uint8_t getControllerNextDirection(Controller * this) {
    float maxPriority = 0.0f;
    uint8_t nextDirection = (this->_dirCurrent + 1) % this->_numOfDirections;
    
    for (uint8_t i = 1; i < this->_numOfDirections; i++) {
        uint8_t direction = (this->_dirCurrent + i) % this->_numOfDirections;
        float priority = getDirectionPriority(this->_directions[direction]);
        
        if (priority > maxPriority) {
            maxPriority = priority;
            nextDirection = direction;
        }
    }
    
	return nextDirection;
}

PUBLIC STATIC void vAutoRunTask(void * pvArg) {
	__RELOAD_WATCHDOG();
	__STARTUP_YELLOW_LAMP_DELAY();
    
    // red lamp while start-up
    for (uint8_t i = 0; i < controller->_numOfDirections; i++) {
        turnRedLamp(&controller->_directions[i]->_lamp);
    }
	
	__RELOAD_WATCHDOG();
	__STARTUP_RED_LAMP_DELAY();
    
	for (;;) {
		xSemaphoreTake(xAutoRunSemaphore, portMAX_DELAY);

		controller->_dirCurrent = controller->_dirNext;
        
        // calculate discharge time
        uint8_t dischargeTime = DEFAULT_GREEN_LAMP_TIME;
        uint8_t requestTime 
                = controller->_directions[controller->_dirCurrent]->_requestTime;
        if (requestTime > MINIMAL_GREEN_LAMP_TIME 
                && requestTime < MAXIMAL_GREEN_LAMP_TIME) {
            dischargeTime = requestTime;
        }

		dischargeDirectionWithCountDown(
            	controller->_directions[controller->_dirCurrent], dischargeTime);
		controller->_dirNext = getControllerNextDirection(controller);
		sendControllerAutoRunMessage(dischargeTime);
	}
}

PUBLIC STATIC void vHandleManualTask(void * pvArg) {
	for (;;) {
		const char * name;
		xQueueReceive(controller->_xManualQueue,  &name, portMAX_DELAY);
		
		if (!strncmp(name, QUIT_STRING, strlen(QUIT_STRING))) {
			quitControllerManualMode();
			continue;
		}
		
		quitControllerAutoRunMode();
		
        // turn red
		int8_t discharge = NONEXISTENT_DIRECTION;
		for (uint8_t i = 0; i < controller->_numOfDirections; i++) {
			if (!strcmp(controller->_directions[i]->_name, name)) {
				discharge = i;
			}

			if (i == controller->_dirCurrent && strcmp(name, ALLRED_STRING)) {
				handleControllerDischargeDirection(controller->_directions[i], 
                        i == discharge);
			} else {
				if (xSemaphoreTake(controller->_directions[i]->_lamp._semphr, 0) == pdTRUE) {
					forbidDirection(controller->_directions[i]);
					xSemaphoreGive(controller->_directions[i]->_lamp._semphr);
				}
			}
		}

        // turn green
		uint8_t roadIndex = 0, laneIndex = 0;
		if (discharge != NONEXISTENT_DIRECTION) {
			controller->_dirCurrent = discharge;
			dischargeDirection(controller->_directions[discharge]);
			getDirectionIndex(controller->_directions[discharge], &roadIndex, &laneIndex);
		}
		
		sendControllerManualMessage(roadIndex, laneIndex);
	}
}

PRIVATE STATIC void sendControllerAutoRunMessage(uint8_t dischargeTime) {
	EthernetObject objects[2] = {
		[0] = {
			.type           = NETOBJ_TIMING_SCHEME_TYPE	, 
			.numOfIndexes	= 2							, 
			.value			= {[0] = dischargeTime}
		}, 
		[1] = {
			.type   		= NETOBJ_TIMING_SCHEME_TYPE	, 
			.numOfIndexes	= 2							, 
			.value			= {[0] = 0x00}
		}
	};

    getDirectionIndex(controller->_directions[controller->_dirCurrent], 
			&objects[0].index[0], &objects[0].index[1]);
	getDirectionIndex(controller->_directions[controller->_dirNext], 
        	&objects[1].index[0], &objects[1].index[1]);
	
	controller->_ethernet->sendMessage(controller->_ethernet, 
        	NETMSG_FORWARDLY_REPORT_TYPE, objects, 2);
}

PRIVATE STATIC void sendControllerManualMessage(uint8_t roadIndex, uint8_t laneIndex) {
	EthernetObject object = {
		.type   		= NETOBJ_TIMING_SCHEME_TYPE				, 
		.numOfIndexes	= 2										, 
		.index			= {[0] = roadIndex, [1] = laneIndex}	, 
		.value			= {[0] = 0xff}
	};

	controller->_ethernet->sendMessage(controller->_ethernet, 
            NETMSG_FORWARDLY_REPORT_TYPE, &object, 1);
}

PRIVATE STATIC void quitControllerManualMode(void) {
	if (xSemaphoreTake(xDirectionDischargeSemphr, 0) == pdTRUE) {
		for (uint8_t i = 0; i < controller->_numOfDirections; i++) {
			if (i == controller->_dirCurrent 
					&& controller->_dirCurrent == controller->_dirNext) {
				continue;
			}

			forbidDirection(controller->_directions[i]);
		}

		xSemaphoreGive(xDirectionDischargeSemphr);
	}
    
	if (xTaskIsTaskSuspended(xAutoRunTask) == pdTRUE) {
		xSemaphoreTake(xAutoRunSemaphore, 0);
        xSemaphoreGive(xAutoRunSemaphore);
        
        vTaskResume(xAutoRunTask);
	}
}

PRIVATE STATIC void quitControllerAutoRunMode(void) {
	xSemaphoreHandle lampSemphr 
			= controller->_directions[controller->_dirCurrent]->_lamp._semphr;
	if (xSemaphoreTake(xDirectionDischargeSemphr, 0) == pdFALSE) {
		if (xSemaphoreTake(lampSemphr, 0) == pdTRUE) {
			vTaskDelete(xDirectionDischargeTask);
			xSemaphoreGive(lampSemphr);
		}
	}

    xSemaphoreGive(xDirectionDischargeSemphr);

	if (xTaskIsTaskSuspended(xAutoRunTask) == pdFALSE) {
		vTaskSuspend(xAutoRunTask);
	}
}

PRIVATE STATIC void handleControllerDischargeDirection(Direction * direction, bool isNext) {
	if (xSemaphoreTake(direction->_lamp._semphr, 0) == pdTRUE) {
		if (!isNext) {
			forbidDirection(direction);
		}

		xSemaphoreGive(direction->_lamp._semphr);
	} else {
		while (direction->_lamp._currentState != &LAMP_STATE_RED) {
			vTaskDelay(100 / portTICK_RATE_MS);
		}
	}
}
