#include "digital/sequential/inc/RegisterGroup.h"

// Private member(s)
PRIVATE STATIC LogFilter filter
    = STATIC_LOG_FILTER("RegisterGroup", LOG_LEVEL_INFO);

// Protected method(s)
PROTECTED void constructBaseScannable(BaseScannable *instance);
PROTECTED void deconstructBaseScannable(BaseScannable *instance);

// Private method(s)
PRIVATE void prepareSerByRegisterGroup(RegisterGroup *pThis, unsigned int data);
PRIVATE void generateSckByRegisterGroup(RegisterGroup *pThis);
PRIVATE void generateRckByRegisterGroup(RegisterGroup *pThis);

// Override method(s)
PUBLIC OVERRIDE void outputRegisterGroupBase(BaseScannable *scannable);

// Virtual methods table
static const BaseScannableVtbl scannableVtbl = {
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
    instance->base.vtbl = &scannableVtbl;

    // 2. construct member(s)
    instance->_oePin = createBasePinByDeviceManager(
        manager, parameter->oePort, parameter->oePin);

    instance->_serPin = createBasePinByDeviceManager(
        manager, parameter->serPort, parameter->serPin);

    instance->_sckPin = createBasePinByDeviceManager(
        manager, parameter->sckPort, parameter->sckPin);

    instance->_rckPin = createBasePinByDeviceManager(
        manager, parameter->rckPort, parameter->rckPin);

    setupBasePin(instance->_oePin, PIN_MODE_OUTPUT);
    setupBasePin(instance->_serPin, PIN_MODE_OUTPUT);
    setupBasePin(instance->_sckPin, PIN_MODE_OUTPUT);
    setupBasePin(instance->_rckPin, PIN_MODE_OUTPUT);

    constructLinkedList(&instance->_shiftRegisters);
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
    destoryBasePinByDeviceManager(manager, instance->_oePin);
    destoryBasePinByDeviceManager(manager, instance->_serPin);
    destoryBasePinByDeviceManager(manager, instance->_sckPin);
    destoryBasePinByDeviceManager(manager, instance->_rckPin);

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
        writeStateToBasePin(pThis->_oePin, PIN_STATE_LOW);
    } else {
        writeStateToBasePin(pThis->_oePin, PIN_STATE_HIGH);
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
        writeStateToBasePin(pThis->_serPin, PIN_STATE_LOW);
    } else {
        writeStateToBasePin(pThis->_serPin, PIN_STATE_HIGH);
    }
}

PRIVATE void generateSckByRegisterGroup(RegisterGroup *pThis)
{
    writeStateToBasePin(pThis->_sckPin, PIN_STATE_LOW);
    writeStateToBasePin(pThis->_sckPin, PIN_STATE_HIGH);
}

PRIVATE void generateRckByRegisterGroup(RegisterGroup *pThis)
{
    writeStateToBasePin(pThis->_rckPin, PIN_STATE_LOW);
    writeStateToBasePin(pThis->_rckPin, PIN_STATE_HIGH);
}

PUBLIC OVERRIDE void outputRegisterGroupBase(BaseScannable *scannable)
{
    LinkedListIterator iterator;
    RegisterGroup *pThis = BaseScannable2RegisterGroup(scannable);

    if (pThis == NULL) {
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

