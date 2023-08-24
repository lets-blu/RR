#include "digital/combinatorial/inc/DataSelectorGroup.h"

// Override method(s)
PROTECTED void scanDataSelectorGroupBase(BaseScannable *scannable);

// Virtual methods table
static const BaseScannableVtbl baseVtbl = {
    .scan = scanDataSelectorGroupBase
};

// Method implements
PUBLIC void constructDataSelectorGroup(
    DataSelectorGroup *instance, DataSelectorGroupParameter *parameter)
{
    DeviceManager *manager = instanceOfDeviceManager();

    if (instance == NULL || parameter == NULL) {
        return;
    }

    // 1. construct base
    constructBaseScannable(&instance->base);
    instance->base.vtbl = &baseVtbl;

    // 2. construct member(s)
    instance->_dataPin = createPinByDeviceManager(
        manager, DEVICE_MANAGER_DIGITAL_PIN, &parameter->dataPin);

    instance->_addressPins = createPinByDeviceManager(
        manager, DEVICE_MANAGER_ADDRESS_PIN, &parameter->addressPins);

    constructLinkedList(&instance->_dataSelectors);

    // 3. Steup pin(s)
    setupBasePin(instance->_dataPin, BASE_PIN_MODE_INPUT);
    setupBasePin(instance->_addressPins, BASE_PIN_MODE_OUTPUT);
}

PUBLIC void deconstructDataSelectorGroup(DataSelectorGroup *instance)
{
    if (instance != NULL) {
        deconstructBaseScannable(&instance->base);
        deconstructLinkedList(&instance->_dataSelectors);
        memset(instance, 0, sizeof(DataSelectorGroup));
    }
}

PUBLIC void addSelectorToDataSelectorGroup(
    DataSelectorGroup *pThis, DataSelector *selector)
{
    if (pThis != NULL && selector != NULL) {
        addItemToLinkedList(&pThis->_dataSelectors, &selector->base);
    }
}

PUBLIC void removeSelectorFromDataSelectorGroup(
    DataSelectorGroup *pThis, DataSelector *selector)
{
    if (pThis != NULL && selector != NULL) {
        removeItemFromLinkedList(&pThis->_dataSelectors, &selector->base);
    }
}

PUBLIC void scanDataSelectorGroup(DataSelectorGroup *pThis)
{
    if (pThis != NULL) {
        scanDataSelectorGroupBase(&pThis->base);
    }
}

PROTECTED void scanDataSelectorGroupBase(BaseScannable *scannable)
{
    LinkedListIterator iterator;
    DataSelectorGroup *pThis = NULL;

    if (scannable == NULL) {
        return;
    }

    pThis = BaseScannable2DataSelectorGroup(scannable);
    constructLinkedListIterator(&iterator, &pThis->_dataSelectors);

    while (hasNextOfLinkedListIterator(&iterator)) {
        DataSelector *selector
            = LinkedListItem2DataSelector(nextOfLinkedListIterator(&iterator));

        unsigned int address = getStartAddressFromDataSelector(selector);

        while (address <= getEndAddressFromDataSelector(selector)) {
            writeToBasePin(pThis->_addressPins, address);

            notifyChangeToDataSelector(
                selector, address, readFromBasePin(pThis->_dataPin));

            address++;
        }
    }

    deconstructLinkedListIterator(&iterator);
}

