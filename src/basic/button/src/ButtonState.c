#include "basic/button/inc/BaseButton.h"
#include "basic/button/inc/ButtonState.h"

// Virtual methods table
static const IButtonStateVtbl buttonStateReleasedVtbl = {
    (IButtonStateOnPushMethod)onPushOnButtonStateReleased,
    (IButtonStateOnReleaseMethod)onReleaseOnButtonStateReleased,
    (IButtonStateToStringMethod)toStringOnButtonStateReleased
};

static const IButtonStateVtbl buttonStateConfirmPushVtbl = {
    (IButtonStateOnPushMethod)onPushOnButtonStateConfirmPush,
    (IButtonStateOnReleaseMethod)onReleaseOnButtonStateConfirmPush,
    (IButtonStateToStringMethod)toStringOnButtonStateConfirmPush
};

static const IButtonStateVtbl buttonStatePushedVtbl = {
    (IButtonStateOnPushMethod)onPushOnButtonStatePushed,
    (IButtonStateOnReleaseMethod)onReleaseOnButtonStatePushed,
    (IButtonStateToStringMethod)toStringOnButtonStatePushed
};

static const IButtonStateVtbl buttonStateConfirmReleaseVtbl = {
    (IButtonStateOnPushMethod)onPushOnButtonStateConfirmRelease,
    (IButtonStateOnReleaseMethod)onReleaseOnButtonStateConfirmRelease,
    (IButtonStateToStringMethod)toStringOnButtonStateConfirmRelease
};

// Private member(s)
PRIVATE STATIC const ButtonStateReleased buttonStateReleased = {
    .buttonState.vtbl = &buttonStateReleasedVtbl
};

PRIVATE STATIC const ButtonStateConfirmPush buttonStateConfirmPush = {
    .buttonState.vtbl = &buttonStateConfirmPushVtbl
};

PRIVATE STATIC const ButtonStatePushed buttonStatePushed = {
    .buttonState.vtbl = &buttonStatePushedVtbl
};

PRIVATE STATIC const ButtonStateConfirmRelease buttonStateConfirmRelease = {
    .buttonState.vtbl = &buttonStateConfirmReleaseVtbl
};

// Public member(s)
PUBLIC STATIC const IButtonState *BUTTON_STATE_RELEASED
    = &buttonStateReleased.buttonState;

PUBLIC STATIC const IButtonState *BUTTON_STATE_CONFIRM_PUSH
    = &buttonStateConfirmPush.buttonState;

PUBLIC STATIC const IButtonState *BUTTON_STATE_PUSHED
    = &buttonStatePushed.buttonState;

PUBLIC STATIC const IButtonState *BUTTON_STATE_CONFIRM_RELEASE
    = &buttonStateConfirmRelease.buttonState;

// Method implement(s)
PUBLIC void constructButtonStateReleased(
    ButtonStateReleased *instance)
{
    if (instance != NULL) {
        instance->buttonState.vtbl = &buttonStateReleasedVtbl;
    }
}

PUBLIC void deconstructButtonStateReleased(
    ButtonStateReleased *instance)
{
    if (instance != NULL) {
        memset(instance, 0, sizeof(ButtonStateReleased));
    }
}

PUBLIC void constructButtonStateConfirmPush(
    ButtonStateConfirmPush *instance)
{
    if (instance != NULL) {
        instance->buttonState.vtbl = &buttonStateConfirmPushVtbl;
    }
}

PUBLIC void deconstructButtonStateConfirmPush(
    ButtonStateConfirmPush *instance)
{
    if (instance != NULL) {
        memset(instance, 0, sizeof(ButtonStateConfirmPush));
    }
}

PUBLIC void constructButtonStatePushed(
    ButtonStatePushed *instance)
{
    if (instance != NULL) {
        instance->buttonState.vtbl = &buttonStatePushedVtbl;
    }
}

PUBLIC void deconstructButtonStatePushed(
    ButtonStatePushed *instance)
{
    if (instance != NULL) {
        memset(instance, 0, sizeof(ButtonStatePushed));
    }
}

PUBLIC void constructButtonStateConfirmRelease(
    ButtonStateConfirmRelease *instance)
{
    if (instance != NULL) {
        instance->buttonState.vtbl = &buttonStateConfirmReleaseVtbl;
    }
}

PUBLIC void deconstructButtonStateConfirmRelease(
    ButtonStateConfirmRelease *instance)
{
    if (instance != NULL) {
        memset(instance, 0, sizeof(ButtonStateConfirmRelease));
    }
}

PUBLIC void onPushOnButtonStateReleased(
    ButtonStateReleased *pThis, BaseButton *button)
{
    if (pThis != NULL && button != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_CONFIRM_PUSH);
    }
}

PUBLIC void onReleaseOnButtonStateReleased(
    ButtonStateReleased *pThis, BaseButton *button)
{
    (void)pThis;
    (void)button;
}

PUBLIC const char *toStringOnButtonStateReleased(
    ButtonStateReleased *pThis)
{
    (void)pThis;
    return "RELEASED";
}

PUBLIC void onPushOnButtonStateConfirmPush(
    ButtonStateConfirmPush *pThis, BaseButton *button)
{
    if (pThis != NULL && button != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_PUSHED);
        button->vtbl->_notifyPush(button);
    }
}

PUBLIC void onReleaseOnButtonStateConfirmPush(
    ButtonStateConfirmPush *pThis, BaseButton *button)
{
    if (pThis != NULL && button != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_RELEASED);
    }
}

PUBLIC const char *toStringOnButtonStateConfirmPush(
    ButtonStateConfirmPush *pThis)
{
    (void)pThis;
    return "CONFIRM_PUSH";
}

PUBLIC void onPushOnButtonStatePushed(
    ButtonStatePushed *pThis, BaseButton *button)
{
    (void)pThis;
    (void)button;
}

PUBLIC void onReleaseOnButtonStatePushed(
    ButtonStatePushed *pThis, BaseButton *button)
{
    if (pThis != NULL && button != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_CONFIRM_RELEASE);
    }
}

PUBLIC const char *toStringOnButtonStatePushed(
    ButtonStatePushed *pThis)
{
    (void)pThis;
    return "PUSHED";
}

PUBLIC void onPushOnButtonStateConfirmRelease(
    ButtonStateConfirmRelease *pThis, BaseButton *button)
{
    if (pThis != NULL && button != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_PUSHED);
    }
}

PUBLIC void onReleaseOnButtonStateConfirmRelease(
    ButtonStateConfirmRelease *pThis, BaseButton *button)
{
    if (pThis != NULL && button != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_RELEASED);
        button->vtbl->_notifyRelease(button);
    }
}

PUBLIC const char *toStringOnButtonStateConfirmRelease(
    ButtonStateConfirmRelease *pThis)
{
    (void)pThis;
    return "CONFIRM_RELEASE";
}

