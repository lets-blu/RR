#include "basic/button/inc/AnalogButtonGroup.h"

// Protected method(s)
PROTECTED void constructBaseScannable(BaseScannable *instance);
PROTECTED void deconstructBaseScannable(BaseScannable *instance);

// Override method(s)
PUBLIC OVERRIDE void scanAnalogButtonGroupBaseScannable(BaseScannable *scannable);

// Virtual methods table
static const BaseScannableVtbl scannableVtbl = {
    .scan = scanAnalogButtonGroupBaseScannable
};

// Method implement(s)
PUBLIC void constructAnalogButtonGroup(
    AnalogButtonGroup *instance, void *port, unsigned int pin)
{
    DeviceManager *manager = instanceOfDeviceManager();

    if (instance == NULL) {
        return;
    }

    // 1. construct base
    constructBaseScannable(&instance->base);
    instance->base.vtbl = &scannableVtbl;

    // 2. construct member(s)
    instance->_pin = createBasePinByDeviceManager(manager, port, pin);
    constructLinkedList(&instance->_analogButtons);

    // 3. setup pin
    setupBasePin(instance->_pin, PIN_MODE_INPUT);
}

PUBLIC void deconstructAnalogButtonGroup(AnalogButtonGroup *instance)
{
    DeviceManager *manager = instanceOfDeviceManager();

    if (instance == NULL) {
        return;
    }

    // 1. deconstruct base
    deconstructBaseScannable(&instance->base);

    // 2. deconstruct member(s)
    destoryBasePinByDeviceManager(manager, instance->_pin);
    deconstructLinkedList(&instance->_analogButtons);

    memset(instance, 0, sizeof(AnalogButtonGroup));
}

PUBLIC void addButtonToAnalogButtonGroup(
    AnalogButtonGroup *pThis, AnalogButton *button)
{
    if (pThis != NULL && button != NULL) {
        addItemToLinkedList(&pThis->_analogButtons, &button->baseItem);
    }
}

PUBLIC void removeButtonFromAnalogButtonGroup(
    AnalogButtonGroup *pThis, AnalogButton *button)
{
    if (pThis != NULL && button != NULL) {
        removeItemFromLinkedList(&pThis->_analogButtons, &button->baseItem);
    }
}

PUBLIC void scanAnalogButtonGroup(AnalogButtonGroup *pThis)
{
    if (pThis != NULL) {
        scanAnalogButtonGroupBaseScannable(&pThis->base);
    }
}

PUBLIC OVERRIDE void scanAnalogButtonGroupBaseScannable(BaseScannable *scannable)
{
    unsigned int value = 0;
    LinkedListIterator iterator;
    AnalogButton *button = NULL;
    AnalogButtonGroup *pThis = BaseScannable2AnalogButtonGroup(scannable);

    if (pThis == NULL) {
        return;
    }

    value = readValueFromBasePin(pThis->_pin);
    constructLinkedListIterator(&iterator, &pThis->_analogButtons);

    while (hasNextOfLinkedListIterator(&iterator)) {
        button = LinkedListItem2AnalogButton(nextOfLinkedListIterator(&iterator));
        notifyValueChangeToAnalogButton(button, value);
    }

    deconstructLinkedListIterator(&iterator);
}

