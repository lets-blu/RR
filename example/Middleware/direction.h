#ifndef __DIRECTION_H__
#define __DIRECTION_H__

#include "FreeRTOS.h"
#include "semphr.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stm32f10x.h"
#include "task.h"

#include "ethernet.h"
#include "ethernetobject.h"
#include "detector.h"
#include "indexes.h"
#include "keyword.h"
#include "lamp.h"
#include "strings.h"
#include "watchdog.h"

#define NUM_OF_DIRECTION_FLOW_DATA	((uint8_t)4)

typedef struct {
	uint8_t time;
	uint16_t numOfVehicles;
} FlowData;

typedef struct Direction {
	const char * _name;
	
	Lamp _lamp;
	uint8_t _countDown;
	
	uint8_t _fdCursor;
	FlowData _arrFlowData[NUM_OF_DIRECTION_FLOW_DATA];
    
    uint8_t _waitingTime, _requestTime;
	
	uint8_t _totalOfDetectors, _numOfDetectors;
	Detector _detectors[];
} Direction;

extern PUBLIC STATIC xTaskHandle xDirectionDischargeTask;
extern PUBLIC STATIC xSemaphoreHandle xDirectionDischargeSemphr;
PUBLIC void staticDirection(void);
PUBLIC Direction * newDirection(const char * name, Lamp lamp, 
		uint8_t totalOfDetectors);
PUBLIC void deleteDirection(Direction * this);

PUBLIC void forbidDirection(Direction * this);
PUBLIC void dischargeDirection(Direction * this);
PUBLIC void dischargeDirectionWithCountDown(Direction * this, uint8_t countDown);
PUBLIC void addDetectorToDirection(Direction * this, 
		Detector detector, DataSelector * selector);
PUBLIC float getDirectionPriority(Direction * this);
PUBLIC void getDirectionIndex(Direction * this, 
		uint8_t * roadIndex, uint8_t * laneIndex);

PUBLIC STATIC void vDirectionDischargTask(void * pvArg);

#endif // __DIRECTION_H__
