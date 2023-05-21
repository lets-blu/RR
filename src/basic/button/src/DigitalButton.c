#include "basic/button/inc/DigitalButton.h"

// Protected method(s)
PROTECTED void constructBaseButton(BaseButton *instance);
PROTECTED void deconstructBaseButton(BaseButton *instance);

PROTECTED void constructBaseScannable(BaseScannable *instance);
PROTECTED void deconstructBaseScannable(BaseScannable *instance);

// Override method(s)
PUBLIC OVERRIDE void notifyPushByDigitalButtonBaseButton(BaseButton *button);
PUBLIC OVERRIDE void notifyReleaseByDigitalButtonBaseButton(BaseButton *button);

PUBLIC OVERRIDE void scanDigitalButtonBaseScannable(BaseScannable *scannable);

// Virtual methods table
static const BaseButtonVtbl buttonVtbl = {
    .notifyPush     = notifyPushByDigitalButtonBaseButton,
    .notifyRelease  = notifyReleaseByDigitalButtonBaseButton
};

static const BaseScannableVtbl scannableVtbl = {
    .scan           = scanDigitalButtonBaseScannable
};

// Method implement(s)
PUBLIC void constructDigitalButton(
    DigitalButton *instance, void *port, unsigned int pin, PinState pushState)
{
    DeviceManager *manager = instanceOfDeviceManager();

    if (instance == NULL) {
        return;
    }

    // 1. construct base
    constructBaseButton(&instance->baseButton);
    instance->baseButton.vtbl = &buttonVtbl;

    constructBaseScannable(&instance->baseScannable);
    instance->baseScannable.vtbl = &scannableVtbl;

    // 2. construct member(s)
    instance->_basePin = createBasePinByDeviceManager(manager, port, pin);
    instance->_pushState = pushState;

    constructLinkedList(&instance->_pushHandlers);
    constructLinkedList(&instance->_releaseHandlers);

    // 3. setup pin
    if (pushState == PIN_STATE_LOW) {
        setupBasePin(instance->_basePin, PIN_MODE_INPUT_PULLUP);
    } else {
        setupBasePin(instance->_basePin, PIN_MODE_INPUT);
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
    destoryBasePinByDeviceManager(manager, instance->_basePin);

    deconstructLinkedList(&instance->_pushHandlers);
    deconstructLinkedList(&instance->_releaseHandlers);

    memset(instance, 0, sizeof(DigitalButton));
}

PUBLIC void addPushHandlerToDigitalButton(
    DigitalButton *pThis, EventHandler *handler)
{
    if (pThis != NULL && handler != NULL) {
        addItemToLinkedList(&pThis->_pushHandlers, &handler->base);
    }
}

PUBLIC void removePushHandlerFromDigitalButton(
    DigitalButton *pThis, EventHandler *handler)
{
    if (pThis != NULL && handler != NULL) {
        removeItemFromLinkedList(&pThis->_pushHandlers, &handler->base);
    }
}

PUBLIC void addReleaseHandlerToDigitalButton(
    DigitalButton *pThis, EventHandler *handler)
{
    if (pThis != NULL && handler != NULL) {
        addItemToLinkedList(&pThis->_releaseHandlers, &handler->base);
    }
}

PUBLIC void removeReleaseHandlerFromDigitalButton(
    DigitalButton *pThis, EventHandler *handler)
{
    if (pThis != NULL && handler != NULL) {
        removeItemFromLinkedList(&pThis->_releaseHandlers, &handler->base);
    }
}

PUBLIC void scanDigitalButton(DigitalButton *pThis)
{
    if (pThis != NULL) {
        scanDigitalButtonBaseScannable(&pThis->baseScannable);
    }
}

PUBLIC OVERRIDE void notifyPushByDigitalButtonBaseButton(BaseButton *button)
{
    LinkedListIterator iterator;
    EventHandler *handler = NULL;
    DigitalButton *pThis = BaseButton2DigitalButton(button);

    if (pThis == NULL) {
        return;
    }

    constructLinkedListIterator(&iterator, &pThis->_pushHandlers);

    while (hasNextOfLinkedListIterator(&iterator)) {
        handler = LinkedListItem2EventHandler(nextOfLinkedListIterator(&iterator));
        handler->callback(handler, pThis, NULL);
    }

    deconstructLinkedListIterator(&iterator);
}

PUBLIC OVERRIDE void notifyReleaseByDigitalButtonBaseButton(BaseButton *button)
{
    LinkedListIterator iterator;
    EventHandler *handler = NULL;
    DigitalButton *pThis = BaseButton2DigitalButton(button);

    if (pThis == NULL) {
        return;
    }

    constructLinkedListIterator(&iterator, &pThis->_releaseHandlers);

    while (hasNextOfLinkedListIterator(&iterator)) {
        handler = LinkedListItem2EventHandler(nextOfLinkedListIterator(&iterator));
        handler->callback(handler, pThis, NULL);
    }

    deconstructLinkedListIterator(&iterator);
}

PUBLIC OVERRIDE void scanDigitalButtonBaseScannable(BaseScannable *scannable)
{
    const IButtonState *currentState = NULL;
    DigitalButton *pThis = BaseScannable2DigitalButton(scannable);

    if (pThis == NULL) {
        return;
    }

    currentState = getStateFromBaseButton(&pThis->baseButton);

    if (readStateFromBasePin(pThis->_basePin) == pThis->_pushState) {
        currentState->vtbl->onPush(
            (IButtonState *)currentState, &pThis->baseButton);
    } else {
        currentState->vtbl->onRelease(
            (IButtonState *)currentState, &pThis->baseButton);
    }
}

