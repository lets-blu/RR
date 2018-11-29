#ifndef __DATASELECTOR_H__
#define __DATASELECTOR_H__

#include "FreeRTOS.h"
#include "queue.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stm32f10x.h"
#include "task.h"

#include "debug.h"
#include "gpiopin.h"
#include "keyword.h"
#include "observer.h"
#include "selectormessage.h"

#define DEFAULT_DATA_SELECTOR (&dataSelector1)

typedef struct DataSelector {
	struct IObserver observer; // implements must be first

	GPIOPin _scanPin;
	uint16_t _scanAddrMask;
	GPIO_TypeDef * _scanAddrPort;

	xQueueHandle _xMessagesQueue;

	struct ISubject ** _subjects;
	uint8_t _totalOfSubjects, _numOfSubjects;
} DataSelector;

extern PUBLIC STATIC DataSelector dataSelector1;
PUBLIC void staticDataSelector(void);
PUBLIC void deleteDataSelector(DataSelector * this);

PUBLIC VIRTUAL void registerSubjectToDataSelector(DataSelector * this, 
		struct ISubject * subject);
PUBLIC VIRTUAL void notifyAllDataSelectorSubjects(DataSelector * this);

PUBLIC STATIC void vScanDataSelectorTask(void * pvArg);
PUBLIC STATIC void vHandleSelectorMessagesTask(void * pvArg);

#endif // __DATASELECTOR_H__
