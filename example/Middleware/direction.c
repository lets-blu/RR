#include "direction.h"

#include "controller.h"

// private method(s)
PRIVATE bool countDownDirection(Direction * this);
PRIVATE void getDirectionVehicles(Direction * this);
PRIVATE uint8_t getDirectionRequestTime(Direction * this);

// static member(s)
PUBLIC STATIC xTaskHandle xDirectionDischargeTask = NULL;
PUBLIC STATIC xSemaphoreHandle xDirectionDischargeSemphr = NULL;

PUBLIC void staticDirection(void) {
	static bool called = false;

	if (called) {
		return;
	}

	staticLamp();
	staticDetector();

	vSemaphoreCreateBinary(xDirectionDischargeSemphr);
	xSemaphoreGive(xDirectionDischargeSemphr);

	called = true;
}

PUBLIC Direction * newDirection(const char * name, Lamp lamp, 
		uint8_t totalOfDetectors) {
	Direction * direction = malloc(sizeof(Direction) 
			+ totalOfDetectors * sizeof(Detector));
	
	if (direction != NULL) {
		direction->_name = name;
		direction->_lamp = lamp;
		direction->_countDown = 0;
		direction->_fdCursor = 0;
        direction->_waitingTime = 0;
        direction->_requestTime = 0;
		direction->_totalOfDetectors = totalOfDetectors;
		direction->_numOfDetectors = 0;
		
		for (uint8_t i = 0; i < NUM_OF_DIRECTION_FLOW_DATA; i++) {
			direction->_arrFlowData[i].time = 0;
			direction->_arrFlowData[i].numOfVehicles = 0;
		}
	}
	
	return direction;
}

PUBLIC void deleteDirection(Direction * this) {
    free(this);
}

PUBLIC void forbidDirection(Direction * this) {
	turnRedLamp(&this->_lamp);
	while (this->_lamp._currentState != &LAMP_STATE_RED) {
		vTaskDelay(100 / portTICK_RATE_MS);
	}

	for (uint8_t i = 0; i < this->_numOfDetectors; i++) {
		setDetectorState(&this->_detectors[i], false);
	}
}

PUBLIC void dischargeDirection(Direction * this) {
	turnGreenLamp(&this->_lamp);

    this->_waitingTime = 0;
	for (uint8_t i = 0; i < this->_numOfDetectors; i++) {
		setDetectorState(&this->_detectors[i], true);
	}
}

PUBLIC void dischargeDirectionWithCountDown(Direction * this, uint8_t countDown) {
	this->_countDown = countDown;
	this->_arrFlowData[this->_fdCursor].time = countDown;
	
	dischargeDirection(this);
	
	xTaskCreate(vDirectionDischargTask, "DirectionDischargeTask", 
            configMINIMAL_STACK_SIZE, this, 1, &xDirectionDischargeTask);
}

PRIVATE bool countDownDirection(Direction * this) {
	if (this->_countDown-- == BLINKING_GREEN_LAMP_TIMES) {
		if (xSemaphoreTake(this->_lamp._semphr, 0) == pdTRUE) {
			forbidDirection(this);
			getDirectionVehicles(this);
			
			xSemaphoreGive(this->_lamp._semphr);
		}
		
		return false;
	}
	
	return true;
}

PUBLIC void addDetectorToDirection(Direction * this, 
		Detector detector, DataSelector * selector) {
	if (this->_numOfDetectors < this->_totalOfDetectors) {
		this->_detectors[this->_numOfDetectors] = detector;
		registerSubjectToDataSelector(selector, 
				(struct ISubject *)(&this->_detectors[this->_numOfDetectors++]));
	}
}

PUBLIC float getDirectionPriority(Direction * this) {
    this->_requestTime = getDirectionRequestTime(this);
    
	return (float)(this->_waitingTime + this->_requestTime) 
            / (float)(this->_requestTime + 1);
}

PRIVATE uint8_t getDirectionRequestTime(Direction * this) {
    float arrLineFitY[NUM_OF_DIRECTION_FLOW_DATA];
    for (uint8_t i = 0; i < NUM_OF_DIRECTION_FLOW_DATA; i++) {
        uint8_t offset = (this->_fdCursor + i) % NUM_OF_DIRECTION_FLOW_DATA;

		if (this->_arrFlowData[offset].time == 0) {
            return 0.0f;
        }
        
        arrLineFitY[i] = (float)this->_arrFlowData[offset].numOfVehicles 
                / (float)this->_arrFlowData[offset].time;
    }
    
    float avgX = 0.0f, avgY = 0.0f;
    float lxx = 0.0f, lyy = 0.0f, lxy = 0.0f;
    
    for (uint8_t i = 0; i < NUM_OF_DIRECTION_FLOW_DATA; i++) {
        avgX += (float)(i + 1) / NUM_OF_DIRECTION_FLOW_DATA;
        avgY += arrLineFitY[i] / NUM_OF_DIRECTION_FLOW_DATA;
    }
    
    for (uint8_t i = 0; i < NUM_OF_DIRECTION_FLOW_DATA; i++) {
        lxx += ((float)(i + 1) - avgX) * ((float)(i + 1) - avgX);
        lyy += (arrLineFitY[i] - avgY) * (arrLineFitY[i] - avgY);
        lxy += ((float)(i + 1) - avgX) * (arrLineFitY[i] - avgY);
    }
    
    float a = lxy / lxx, b = avgY - lxy * avgX / lxx;
    
    uint8_t lastFlowDataOffset = (this->_fdCursor == 0 ? 
            NUM_OF_DIRECTION_FLOW_DATA : this->_fdCursor) - 1;
    float flowIncrement = a * (NUM_OF_DIRECTION_FLOW_DATA + 1) + b 
            - arrLineFitY[NUM_OF_DIRECTION_FLOW_DATA - 1];
    float requestTime = this->_arrFlowData[lastFlowDataOffset].time 
            * (1 + flowIncrement / (arrLineFitY[NUM_OF_DIRECTION_FLOW_DATA - 1] + 1));

    return requestTime - (int)requestTime > 0.5 ? 
            (int)(requestTime + 1) : (int)requestTime;
}

PUBLIC void getDirectionIndex(Direction * this, 
		uint8_t * roadIndex, uint8_t * laneIndex) {
	uint8_t offset = 0;
	
	uint8_t ewStrlen = strlen(__STRING(EASTWEST));
	uint8_t snStrlen = strlen(__STRING(SOUTHNORTH));
	if (!strncmp(this->_name, __STRING(EASTWEST), ewStrlen)) {
		*roadIndex = EASTWEST_INDEX;
		offset = ewStrlen;
	} else {
		if (!strncmp(this->_name, __STRING(SOUTHNORTH), snStrlen)) {
			* roadIndex = SOUTHNORTH_INDEX;
			offset = snStrlen;
		}
	}
	
	uint8_t sStrlen = strlen(__STRING(STRAIGHT));
	uint8_t ltStrlen = strlen(__STRING(LEFTTURN));
	if (!strncmp(this->_name + offset, __STRING(STRAIGHT), sStrlen)) {
		*laneIndex = STRAIGHT_INDEX;
	} else {
		if (!strncmp(this->_name + offset, __STRING(LEFTTURN), ltStrlen)) {
			*laneIndex = LEFTTURN_INDEX;
		}
	}
}

PRIVATE void getDirectionVehicles(Direction * this) {
	this->_arrFlowData[this->_fdCursor].numOfVehicles = 0;
	for (uint8_t i = 0; i < this->_numOfDetectors; i++) {
		this->_arrFlowData[this->_fdCursor].numOfVehicles 
				+= getDetectorStandardVehicles(&this->_detectors[i]);
		resetDetector(&this->_detectors[i]);
	}
	
	this->_fdCursor = (this->_fdCursor + 1) % NUM_OF_DIRECTION_FLOW_DATA;
}

PUBLIC STATIC void vDirectionDischargTask(void * pvArg) {
	Direction * direction = (Direction *)pvArg;
	
	xSemaphoreTake(xDirectionDischargeSemphr, 0);
	
	while (countDownDirection(direction)) {
		__RELOAD_WATCHDOG();
		portTickType xLastWeakTime = xTaskGetTickCount();
		
        for (uint8_t i = 0; i < controller->_numOfDirections; i++) {
            controller->_directions[i]->_waitingTime++;
        }
		
		vTaskDelayUntil(&xLastWeakTime, 1000 / portTICK_RATE_MS);
	}

	xSemaphoreGive(xAutoRunSemaphore);
	xSemaphoreGive(xDirectionDischargeSemphr);

	vTaskDelete(NULL);
}
