#include "dataselector.h"

#define DEFAULT_QUEUE_LENGTH 32

// (de)constructor(s)
PRIVATE DataSelector newDataSelector(uint8_t totalOfSubjects);

// private method(s)
PRIVATE void scanDataSelector(DataSelector * this, uint16_t address);
PRIVATE void setDataSelectorScanPins(DataSelector * this, 
		GPIOPin scanPin, GPIOPin scanAddrPins);
PRIVATE uint8_t getDataSelectorScanAddrOffset(DataSelector * this);

// static member(s)
PUBLIC STATIC DataSelector dataSelector1;

PUBLIC void staticDataSelector(void) {
	static bool called = false;

	if (called) {
		return;
	}

	dataSelector1 = newDataSelector(16);
	setDataSelectorScanPins(&dataSelector1, newGPIOPin(GPIOB, GPIO_Pin_5), 
			newGPIOPin(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9));
	
	called = true;
}

PRIVATE DataSelector newDataSelector(uint8_t totalOfSubjects) {
	DataSelector selector;

	selector.observer.registerSubject
				= (void (*)(struct IObserver *, struct ISubject *))registerSubjectToDataSelector;
	selector.observer.removeSubject
				= (void (*)(struct IObserver *, struct ISubject *))NULL;
	selector.observer.notifyAllSubjects
				= (void (*)(struct IObserver *))notifyAllDataSelectorSubjects;
	
	selector._xMessagesQueue = xQueueCreate(DEFAULT_QUEUE_LENGTH, sizeof(SelectorMessage));
#ifdef DEBUG
	vQueueAddToRegistry(selector._xMessagesQueue, "DataSelectorMessagesQueue");
#endif

	selector._subjects = malloc(totalOfSubjects * sizeof(struct ISubject *));
	selector._numOfSubjects = 0;
	selector._totalOfSubjects = selector._subjects == NULL ? 0 : totalOfSubjects;
	
	return selector;
}

PUBLIC void deleteDataSelector(DataSelector * this) {
	free(this->_subjects);
}

PUBLIC VIRTUAL void registerSubjectToDataSelector(DataSelector * this, 
		struct ISubject * subject) {
	if (this->_numOfSubjects < this->_totalOfSubjects) {
		taskENTER_CRITICAL();
		{
			this->_subjects[this->_numOfSubjects++] = subject;
		}
		taskEXIT_CRITICAL();
	}
}

PUBLIC VIRTUAL void notifyAllDataSelectorSubjects(DataSelector * this) {
	for (uint8_t i = 0; i < this->_numOfSubjects; i++) {
		if (this->_subjects[i]->update != NULL) {
			this->_subjects[i]->update((struct ISubject *)this->_subjects[i], 
					(struct IObserver *)this);
		}
	}
}

PRIVATE void scanDataSelector(DataSelector * this, uint16_t address) {
	this->_scanAddrPort->ODR &= ~(this->_scanAddrMask);
	this->_scanAddrPort->ODR |= address << getDataSelectorScanAddrOffset(this);

	SelectorMessage message = newSelectorMessage(address, 
			(GPIOPinState)GPIO_ReadInputDataBit(this->_scanPin._port, this->_scanPin._pin));
	xQueueSendToBack(this->_xMessagesQueue, &message, 0);
}

PRIVATE void setDataSelectorScanPins(DataSelector * this, 
		GPIOPin scanPin, GPIOPin scanAddrPins) {
	configGPIOPin(&scanPin, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING);
	
	this->_scanPin = scanPin;
	this->_scanAddrPort = scanAddrPins._port;
	this->_scanAddrMask = scanAddrPins._pin;
	configGPIOPin(&scanAddrPins, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
}

PRIVATE uint8_t getDataSelectorScanAddrOffset(DataSelector * this) {
	uint8_t offset = 0;
    for (uint16_t i = 0x0001; !(this->_scanAddrMask & i); i <<= 1, offset++);

    return offset;
}

PUBLIC STATIC void vScanDataSelectorTask(void * pvArg) {
	DataSelector * selector = (DataSelector *)pvArg;

	for (;;) {
		for (uint16_t address = 0x0000; address < 0x0010; address++) {
			scanDataSelector(selector, address);
		}

		vTaskDelay(100 / portTICK_RATE_MS);
	}
}

PUBLIC STATIC void vHandleSelectorMessagesTask(void * pvArg) {
	SelectorMessage message;
	DataSelector * selector = (DataSelector *)pvArg;

	for (;;) {
		xQueuePeek(selector->_xMessagesQueue, &message, portMAX_DELAY);
		
		notifyAllDataSelectorSubjects(selector);
		xQueueReceive(selector->_xMessagesQueue, &message, 0);
	}
}
