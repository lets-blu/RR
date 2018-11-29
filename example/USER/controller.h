#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stm32f10x.h"
#include "string.h"
#include "task.h"

#include "debug.h"
#include "direction.h"
#include "ethernet.h"
#include "keyword.h"
#include "manual.h"
#include "strings.h"

typedef struct Controller {
	Manual _manual;
	struct Ethernet * _ethernet;
	xQueueHandle _xManualQueue;
	
	uint8_t _dirCurrent, _dirNext;
	
	uint8_t _totalOfDirections, _numOfDirections; 
	Direction * _directions[];
} Controller;

extern PUBLIC STATIC Controller * controller;
extern PUBLIC STATIC xTaskHandle xAutoRunTask;
extern PUBLIC STATIC xSemaphoreHandle xAutoRunSemaphore;
PUBLIC void staticController(void);
PUBLIC void deleteController(Controller * this);

PUBLIC void setControllerManual(Controller * this, Manual manual);
PUBLIC void setControllerEthernet(Controller * this, struct Ethernet * ethernet);
PUBLIC void addDirectionToController(Controller * this, Direction * direction);

PUBLIC STATIC void vAutoRunTask(void * pvArg);
PUBLIC STATIC void vHandleManualTask(void * pvArg);

#endif // __CONTROLLER_H__
