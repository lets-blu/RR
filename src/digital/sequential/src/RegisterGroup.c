#include "digital/sequential/inc/RegisterGroup.h"

// Private member(s)
PRIVATE STATIC LogFilter filter
    = STATIC_LOG_FILTER("RegisterGroup", LOG_LEVEL_INFO);

// Private method(s)
PRIVATE void prepareSerByRegisterGroup(RegisterGroup *pThis, unsigned int data);
PRIVATE void generateSckByRegisterGroup(RegisterGroup *pThis);
PRIVATE void generateRckByRegisterGroup(RegisterGroup *pThis);

// Override method(s)
PUBLIC OVERRIDE void outputRegisterGroupBase(BaseScannable *scannable);

// Virtual methods table
static const BaseScannableVtbl baseVtbl = {
    .scan = outputRegisterGroupBase
};

// Method implement(s)
PUBLIC void constructRegisterGroup(
    RegisterGroup *instance, RegisterGroupParameter *parameter)
{
    DeviceManager *manager = instanceOfDeviceManager();

    if (instance == NULL || parameter == NULL) {
        return;
    }

    // 1. construct base
    constructBaseScannable(&instance->base);
    instance->base.vtbl = &baseVtbl;

    // 2. construct member(s)
    instance->_oePin = createPinByDeviceManager(
        manager, DEVICE_MANAGER_DIGITAL_PIN, &parameter->oePin);

    instance->_serPin = createPinByDeviceManager(
        manager, DEVICE_MANAGER_DIGITAL_PIN, &parameter->serPin);

    instance->_sckPin = createPinByDeviceManager(
        manager, DEVICE_MANAGER_DIGITAL_PIN, &parameter->sckPin);

    instance->_rckPin = createPinByDeviceManager(
        manager, DEVICE_MANAGER_DIGITAL_PIN, &parameter->rckPin);

    constructLinkedList(&instance->_shiftRegisters);

    // 3. setup pin(s)
    setupBasePin(instance->_oePin, BASE_PIN_MODE_OUTPUT);
    setupBasePin(instance->_serPin, BASE_PIN_MODE_OUTPUT);
    setupBasePin(instance->_sckPin, BASE_PIN_MODE_OUTPUT);
    setupBasePin(instance->_rckPin, BASE_PIN_MODE_OUTPUT);
}

PUBLIC void deconstructRegisterGroup(RegisterGroup *instance)
{
    DeviceManager *manager = instanceOfDeviceManager();

    if (instance == NULL) {
        return;
    }

    // 1. deconstruct base
    deconstructBaseScannable(&instance->base);

    // 2. deconstruct member(s)
    destoryPinByDeviceManager(
        manager, DEVICE_MANAGER_DIGITAL_PIN, instance->_oePin);

    destoryPinByDeviceManager(
        manager, DEVICE_MANAGER_DIGITAL_PIN, instance->_serPin);

    destoryPinByDeviceManager(
        manager, DEVICE_MANAGER_DIGITAL_PIN, instance->_sckPin);

    destoryPinByDeviceManager(
        manager, DEVICE_MANAGER_DIGITAL_PIN, instance->_rckPin);

    deconstructLinkedList(&instance->_shiftRegisters);
}

PUBLIC void addRegisterToRegisterGroup(
    RegisterGroup *pThis, ShiftRegister *shiftRegister)
{
    if (pThis != NULL && shiftRegister != NULL) {
        addItemToLinkedList(&pThis->_shiftRegisters, &shiftRegister->base);
    }
}

PUBLIC void removeRegisterFromRegisterGroup(
    RegisterGroup *pThis, ShiftRegister *shiftRegister)
{
    if (pThis != NULL && shiftRegister != NULL) {
        removeItemFromLinkedList(&pThis->_shiftRegisters, &shiftRegister->base);
    }
}

PUBLIC void setOutputEnableToRegisterGroup(
    RegisterGroup *pThis, bool enable)
{
    if (pThis == NULL) {
        return;
    }

    if (enable) {
        writeToBasePin(pThis->_oePin, BASE_PIN_VALUE_LOW);
    } else {
        writeToBasePin(pThis->_oePin, BASE_PIN_VALUE_HIGH);
    }
}

PUBLIC void outputRegisterGroup(RegisterGroup *pThis)
{
    if (pThis != NULL) {
        outputRegisterGroupBase(&pThis->base);
    }
}

PRIVATE void prepareSerByRegisterGroup(RegisterGroup *pThis, unsigned int data)
{
    if (data == 0) {
        writeToBasePin(pThis->_serPin, BASE_PIN_VALUE_LOW);
    } else {
        writeToBasePin(pThis->_serPin, BASE_PIN_VALUE_HIGH);
    }
}

PRIVATE void generateSckByRegisterGroup(RegisterGroup *pThis)
{
    writeToBasePin(pThis->_sckPin, BASE_PIN_VALUE_LOW);
    writeToBasePin(pThis->_sckPin, BASE_PIN_VALUE_HIGH);
}

PRIVATE void generateRckByRegisterGroup(RegisterGroup *pThis)
{
    writeToBasePin(pThis->_rckPin, BASE_PIN_VALUE_LOW);
    writeToBasePin(pThis->_rckPin, BASE_PIN_VALUE_HIGH);
}

PUBLIC OVERRIDE void outputRegisterGroupBase(BaseScannable *scannable)
{
    LinkedListIterator iterator;
    RegisterGroup *pThis = BaseScannable2RegisterGroup(scannable);

    if (scannable == NULL) {
        return;
    }

    constructLinkedListIterator(&iterator, &pThis->_shiftRegisters);

    while (hasNextOfLinkedListIterator(&iterator)) {
        ShiftRegister *shiftRegister = LinkedListItem2ShiftRegister(
            nextOfLinkedListIterator(&iterator));

        unsigned int data = getDataFromShiftRegister(shiftRegister);
        unsigned int dataSize = getDataSizeFromShiftRegister(shiftRegister);

        LOG_I(&filter, "output, data of register 0x%x is 0x%02x",
            shiftRegister, data);

        for (unsigned int i = 0; i < dataSize; i++) {
            prepareSerByRegisterGroup(pThis, data & (1 << (dataSize - 1)));
            generateSckByRegisterGroup(pThis);
            data <<= 1;
        }
    }

    generateRckByRegisterGroup(pThis);
    deconstructLinkedListIterator(&iterator);
}

