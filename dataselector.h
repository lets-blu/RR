#ifndef __DATASELECTOR_H__
#define __DATASELECTOR_H__

#include "FreeRTOS.h"
#include "queue.h"
#include "stdlib.h"
#include "stm32f10x.h"
#include "task.h"

#include "gpiopin.h"
#include "keywords.h"
#include "observer.h"
#include "selectormessage.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    typedef struct DataSelector {
        struct IObserver observer;

        GPIOPin _scanPin;
        GPIOPin _scanAddrPins;

        xQueueHandle _xMessagesQueue;

        struct ISubject ** _subjects;
        uint8_t _totalOfSubjects, _numOfSubjects;
    } DataSelector;

    // (de)constructor(s)
    PUBLIC DataSelector newDataSelector(uint8_t totalOfSubjects);
    PUBLIC void deleteDataSelector(DataSelector * pThis);

    // public method(s)
    PUBLIC void setDataSelectorPins(DataSelector * pThis,
        GPIOPin scanPin, GPIOPin scanAddrPins);
    PUBLIC void scanDataSelector(DataSelector * pThis, uint8_t address);
    PUBLIC VIRTUAL void registerSubjectToDataSelector(DataSelector * pThis,
        struct ISubject * subject);
    PUBLIC VIRTUAL void removeSubjectFromDataSelector(DataSelector * pThis,
        struct ISubject * subject);
    PUBLIC VIRTUAL void notifyAllDataSelectorSubjects(DataSelector * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DATASELECTOR_H__
