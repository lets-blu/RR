#include "dataselector.h"

#define __QUEUE_LENGTH  32
#define __READ_INTERVAL 10

// private function(s)
PRIVATE uint8_t getDataSelectorScanAddrOffset(DataSelector * pThis);

PUBLIC DataSelector newDataSelector(uint8_t totalOfSubjects) {
    DataSelector selector = {
        .observer.registerSubject
            = (register_subject_fp)registerSubjectToDataSelector,
        .observer.removeSubject
            = (remove_subject_fp)removeSubjectFromDataSelector,
        .observer.notifyAllSubjects
            = (notify_subjects_fp)notifyAllDataSelectorSubjects,

        ._xMessagesQueue = NULL,

        ._subjects = NULL,
        ._totalOfSubjects = 0,
        ._numOfSubjects = 0
    };

    selector._xMessagesQueue = xQueueCreate(__QUEUE_LENGTH, sizeof(SelectorMessage));
#ifdef DEBUG
    vQueueAddToRegistry(selector._xMessagesQueue, "DataSelectorMessagesQueue");
#endif // DEBUG

    selector._subjects = malloc(totalOfSubjects * sizeof(struct ISubject *));
    selector._totalOfSubjects = selector._subjects == NULL ? 0 : totalOfSubjects;

    return selector;
}

PUBLIC void deleteDataSelector(DataSelector * pThis) {
    free(pThis->_subjects);
}

PUBLIC void setDataSelectorPins(DataSelector * pThis,
    GPIOPin scanPin, GPIOPin scanAddrPins) {
    pThis->_scanPin = scanPin;
    configGPIOPin(&pThis->_scanPin, GPIO_Speed_2MHz, GPIO_Mode_IN_FLOATING);

    pThis->_scanAddrPins = scanAddrPins;
    configGPIOPin(&pThis->_scanAddrPins, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
}

PUBLIC void scanDataSelector(DataSelector * pThis, uint8_t address) {
    uint8_t offset = getDataSelectorScanAddrOffset(pThis);
    pThis->_scanAddrPins._port->ODR &= ~(pThis->_scanAddrPins._pin);
    pThis->_scanAddrPins._port->ODR |= address << offset;

    vTaskDelay(__READ_INTERVAL / portTICK_RATE_MS);

    GPIOPinState data = readGPIOPin(&pThis->_scanPin);
    SelectorMessage message = newSelectorMessage(address, data);
    xQueueSendToBack(pThis->_xMessagesQueue, &message, 0);
}

PUBLIC VIRTUAL void registerSubjectToDataSelector(DataSelector * pThis,
    struct ISubject * subject) {
    if (pThis->_numOfSubjects < pThis->_totalOfSubjects) {
        pThis->_subjects[pThis->_numOfSubjects++] = subject;
    }
}

PUBLIC VIRTUAL void removeSubjectFromDataSelector(DataSelector * pThis,
    struct ISubject * subject) {
    // not supported yet
}

PUBLIC VIRTUAL void notifyAllDataSelectorSubjects(DataSelector * pThis) {
    for (uint8_t i = 0; i < pThis->_numOfSubjects; ++i) {
        pThis->_subjects[i]->update(pThis->_subjects[i], (struct IObserver *)pThis);
    }
}

PRIVATE uint8_t getDataSelectorScanAddrOffset(DataSelector * pThis) {
    uint8_t offset = 0;
    for (uint16_t i = 0x00001; !(pThis->_scanAddrPins._pin & i); i <<= 1, ++offset);

    return offset;
}
