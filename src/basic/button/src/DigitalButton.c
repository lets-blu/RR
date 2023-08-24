#include "basic/button/inc/DigitalButton.h"

// Override method(s)
PUBLIC OVERRIDE void scanDigitalButtonBaseScannable(BaseScannable *scannable);

// Virtual methods table
static const BaseScannableVtbl baseScannableVtbl = {
    .scan = scanDigitalButtonBaseScannable
};

// Method implement(s)
PUBLIC void constructDigitalButton(
    DigitalButton *instance,
    BasePinParameter *parameter,
    unsigned int pushState)
{
    DeviceManager *manager = instanceOfDeviceManager();

    if (instance == NULL) {
        return;
    }

    // 1. construct base
    constructBaseButton(&instance->baseButton);

    constructBaseScannable(&instance->baseScannable);
    instance->baseScannable.vtbl = &baseScannableVtbl;

    // 2. construct member(s)
    instance->_pin = createPinByDeviceManager(
        manager, DEVICE_MANAGER_DIGITAL_PIN, parameter);

    instance->_pushState = pushState;

    // 3. setup pin
    if (pushState == BASE_PIN_VALUE_LOW) {
        setupBasePin(instance->_pin, BASE_PIN_MODE_INPUT_PULLUP);
    } else {
        setupBasePin(instance->_pin, BASE_PIN_MODE_INPUT);
    }
}

PUBLIC void deconstructDigitalButton(DigitalButton *instance)
{
    DeviceManager *manager = instanceOfDeviceManager();

    if (instance == NULL) {
        return;
    }

    // 1. deconstruct base
    deconstructBaseButton(&instance->baseButton);
    deconstructBaseScannable(&instance->baseScannable);

    // 2. deconstruct member(s)
    destoryPinByDeviceManager(
        manager, DEVICE_MANAGER_DIGITAL_PIN, instance->_pin);

    memset(instance, 0, sizeof(DigitalButton));
}

PUBLIC void addClickHandlerToDigitalButton(
    DigitalButton *pThis, EventHandler *handler)
{
    if (pThis != NULL) {
        addClickHandlerToBaseButton(&pThis->baseButton, handler);
    }
}

PUBLIC void removeClickHandlerFromDigitalButton(
    DigitalButton *pThis, EventHandler *handler)
{
    if (pThis != NULL) {
        removeClickHandlerFromBaseButton(&pThis->baseButton, handler);
    }
}

PUBLIC void scanDigitalButton(DigitalButton *pThis)
{
    if (pThis != NULL) {
        scanDigitalButtonBaseScannable(&pThis->baseScannable);
    }
}

PUBLIC OVERRIDE void scanDigitalButtonBaseScannable(BaseScannable *scannable)
{
    const ButtonState *currentState = NULL;
    DigitalButton *pThis = NULL;

    if (scannable == NULL) {
        return;
    }

    pThis = BaseScannable2DigitalButton(scannable);
    currentState = getStateFromBaseButton(&pThis->baseButton);

    if (readFromBasePin(pThis->_pin) == pThis->_pushState) {
        onPushOnButtonState((ButtonState *)currentState, &pThis->baseButton);
    } else {
        onReleaseOnButtonState((ButtonState *)currentState, &pThis->baseButton);
    }
}

