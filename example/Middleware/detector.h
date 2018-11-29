#ifndef __DETECTOR_H__
#define __DETECTOR_H__

#include "stdbool.h"
#include "stdlib.h"
#include "stm32f10x.h"

#include "dataselector.h"
#include "gpiopin.h"
#include "observer.h"
#include "selectormessage.h"

typedef struct Detector {
	struct ISubject subject; // must be first

    bool _countEnable;
	GPIOPinState _lastDetect;
	
	uint8_t _id, _weight, _numOfVehicles;
} Detector;

PUBLIC void staticDetector(void);
PUBLIC Detector newDetector(uint8_t id, uint8_t weight);

PUBLIC void resetDetector(Detector * this);
PUBLIC void setDetectorState(Detector * this, bool countEnable);
PUBLIC uint16_t getDetectorStandardVehicles(Detector * this);
PUBLIC VIRTUAL void updateDetector(Detector * this, struct IObserver * observer);

#endif // __DETECTOR_H__
