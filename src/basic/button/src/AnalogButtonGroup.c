#include "basic/button/inc/AnalogButtonGroup.h"

// Override method(s)
PUBLIC OVERRIDE void scanAnalogButtonGroupBase(BaseScannable *scannable);

// Virtual methods table
static const BaseScannableVtbl baseVtbl = {
    .scan = scanAnalogButtonGroupBase
};

// Method implement(s)
PUBLIC void constructAnalogButtonGroup(
    AnalogButtonGroup *instance, BasePinParameter *parameter)
{
    DeviceManager *manager = instanceOfDeviceManager();

    if (instance == NULL) {
        return;
    }

    // 1. construct base
    constructBaseScannable(&instance->base);
    instance->base.vtbl = &baseVtbl;

    // 2. construct member(s)
    instance->_pin = createPinByDeviceManager(
        manager, DEVICE_MANAGER_ANALOG_PIN, parameter);

    constructLinkedList(&instance->_analogButtons);

    // 3. setup pin
    setupBasePin(instance->_pin, BASE_PIN_MODE_INPUT);
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
    destoryPinByDeviceManager(
        manager, DEVICE_MANAGER_ANALOG_PIN, instance->_pin);

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
        scanAnalogButtonGroupBase(&pThis->base);
    }
}

PUBLIC OVERRIDE void scanAnalogButtonGroupBase(BaseScannable *scannable)
{
    unsigned int value = 0;
    LinkedListIterator iterator;
    AnalogButtonGroup *pThis = NULL;

    if (scannable == NULL) {
        return;
    }

    pThis = BaseScannable2AnalogButtonGroup(scannable);
    value = readFromBasePin(pThis->_pin);
    constructLinkedListIterator(&iterator, &pThis->_analogButtons);

    while (hasNextOfLinkedListIterator(&iterator)) {
        AnalogButton *button
            = LinkedListItem2AnalogButton(nextOfLinkedListIterator(&iterator));

        notifyValueChangeToAnalogButton(button, value);
    }

    deconstructLinkedListIterator(&iterator);
}

