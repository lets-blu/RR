#include "detector.h"

// private method(s)
PRIVATE void acceptSelectorMessage(Detector * this, SelectorMessage message);

PUBLIC void staticDetector(void) {
	static bool called = false;

	if (called) {
		return;
	}

	staticDataSelector();

	called = true;
}

PUBLIC Detector newDetector(uint8_t id, uint8_t weight) {
	Detector detector = {
		.subject.update
					= (void (*)(struct ISubject *, struct IObserver *))updateDetector	,
		._id		= id															    , 
		._weight	= weight
	};

	resetDetector(&detector);
	
	return detector;
}

PUBLIC void resetDetector(Detector * this) {
	this->_numOfVehicles = 0;
}

PUBLIC void setDetectorState(Detector * this, bool countEnable) {
    this->_countEnable = countEnable;
}

PUBLIC uint16_t getDetectorStandardVehicles(Detector * this) {
	return this->_numOfVehicles * this->_weight;
}

PUBLIC VIRTUAL void updateDetector(Detector * this, struct IObserver * observer) {
	SelectorMessage message;
	DataSelector * selector = (DataSelector *)observer;

	if (xQueuePeek(selector->_xMessagesQueue, &message, 0) == pdTRUE 
			&& message.id == this->_id) {
		acceptSelectorMessage(this, message);
	}
}

PRIVATE void acceptSelectorMessage(Detector * this, SelectorMessage message) {
    if (this->_countEnable && this->_lastDetect == HIGH && message.data == LOW) {
        this->_numOfVehicles++;
    }

	this->_lastDetect = message.data;
}
