#include "basic/button/inc/BaseButton.h"
#include "basic/button/inc/ButtonState.h"

// Override method(s)
PUBLIC OVERRIDE void onPushOnButtonStateReleased(
    ButtonState *pThis, struct BaseButton *button);

PUBLIC OVERRIDE void onReleaseOnButtonStateReleased(
    ButtonState *pThis, struct BaseButton *button);

PUBLIC OVERRIDE const char *toStringOnButtonStateReleased(
    ButtonState *pThis);

PUBLIC OVERRIDE void onPushOnButtonStateConfirmPush(
    ButtonState *pThis, struct BaseButton *button);

PUBLIC OVERRIDE void onReleaseOnButtonStateConfirmPush(
    ButtonState *pThis, struct BaseButton *button);

PUBLIC OVERRIDE const char *toStringOnButtonStateConfirmPush(
    ButtonState *pThis);

PUBLIC OVERRIDE void onPushOnButtonStatePushed(
    ButtonState *pThis, struct BaseButton *button);

PUBLIC OVERRIDE void onReleaseOnButtonStatePushed(
    ButtonState *pThis, struct BaseButton *button);

PUBLIC OVERRIDE const char *toStringOnButtonStatePushed(
    ButtonState *pThis);

PUBLIC OVERRIDE void onPushOnButtonStateConfirmRelease(
    ButtonState *pThis, struct BaseButton *button);

PUBLIC OVERRIDE void onReleaseOnButtonStateConfirmRelease(
    ButtonState *pThis, struct BaseButton *button);

PUBLIC OVERRIDE const char *toStringOnButtonStateConfirmRelease(
    ButtonState *pThis);

// Virtual methods table
static const ButtonStateVtbl buttonStateReleasedVtbl = {
    .onPush     = onPushOnButtonStateReleased,
    .onRelease  = onReleaseOnButtonStateReleased,
    .toString   = toStringOnButtonStateReleased
};

static const ButtonStateVtbl buttonStateConfirmPushVtbl = {
    .onPush     = onPushOnButtonStateConfirmPush,
    .onRelease  = onReleaseOnButtonStateConfirmPush,
    .toString   = toStringOnButtonStateConfirmPush
};

static const ButtonStateVtbl buttonStatePushedVtbl = {
    .onPush     = onPushOnButtonStatePushed,
    .onRelease  = onReleaseOnButtonStatePushed,
    .toString   = toStringOnButtonStatePushed
};

static const ButtonStateVtbl buttonStateConfirmReleaseVtbl = {
    .onPush     = onPushOnButtonStateConfirmRelease,
    .onRelease  = onReleaseOnButtonStateConfirmRelease,
    .toString   = toStringOnButtonStateConfirmRelease
};

// Private member(s)
PRIVATE STATIC const ButtonState buttonStateReleased = {
    .vtbl = &buttonStateReleasedVtbl
};

PRIVATE STATIC const ButtonState buttonStateConfirmPush = {
    .vtbl = &buttonStateConfirmPushVtbl
};

PRIVATE STATIC const ButtonState buttonStatePushed = {
    .vtbl = &buttonStatePushedVtbl
};

PRIVATE STATIC const ButtonState buttonStateConfirmRelease = {
    .vtbl = &buttonStateConfirmReleaseVtbl
};

// Public member(s)
PUBLIC STATIC const ButtonState *BUTTON_STATE_RELEASED
    = &buttonStateReleased;

PUBLIC STATIC const ButtonState *BUTTON_STATE_CONFIRM_PUSH
    = &buttonStateConfirmPush;

PUBLIC STATIC const ButtonState *BUTTON_STATE_PUSHED
    = &buttonStatePushed;

PUBLIC STATIC const ButtonState *BUTTON_STATE_CONFIRM_RELEASE
    = &buttonStateConfirmRelease;

// Method implement(s)
PUBLIC void onPushOnButtonState(
    ButtonState *pThis, struct BaseButton *button)
{
    if (pThis != NULL) {
        pThis->vtbl->onPush(pThis, button);
    }
}

PUBLIC void onReleaseOnButtonState(
    ButtonState *pThis, struct BaseButton *button)
{
    if (pThis != NULL) {
        pThis->vtbl->onRelease(pThis, button);
    }
}

PUBLIC const char *toStringOnButtonState(ButtonState *pThis)
{
    return (pThis == NULL) ? NULL : pThis->vtbl->toString(pThis);
}

PUBLIC OVERRIDE void onPushOnButtonStateReleased(
    ButtonState *pThis, struct BaseButton *button)
{
    if (pThis != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_CONFIRM_PUSH);
    }
}

PUBLIC OVERRIDE void onReleaseOnButtonStateReleased(
    ButtonState *pThis, struct BaseButton *button)
{
    (void)pThis;
    (void)button;
}

PUBLIC OVERRIDE const char *toStringOnButtonStateReleased(
    ButtonState *pThis)
{
    (void)pThis;
    return "RELEASED";
}

PUBLIC OVERRIDE void onPushOnButtonStateConfirmPush(
    ButtonState *pThis, struct BaseButton *button)
{
    if (pThis != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_PUSHED);
    }
}

PUBLIC OVERRIDE void onReleaseOnButtonStateConfirmPush(
    ButtonState *pThis, struct BaseButton *button)
{
    if (pThis != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_RELEASED);
    }
}

PUBLIC OVERRIDE const char *toStringOnButtonStateConfirmPush(
    ButtonState *pThis)
{
    (void)pThis;
    return "CONFIRM_PUSH";
}

PUBLIC OVERRIDE void onPushOnButtonStatePushed(
    ButtonState *pThis, struct BaseButton *button)
{
    (void)pThis;
    (void)button;
}

PUBLIC OVERRIDE void onReleaseOnButtonStatePushed(
    ButtonState *pThis, struct BaseButton *button)
{
    if (pThis != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_CONFIRM_RELEASE);
    }
}

PUBLIC OVERRIDE const char *toStringOnButtonStatePushed(
    ButtonState *pThis)
{
    (void)pThis;
    return "PUSHED";
}

PUBLIC OVERRIDE void onPushOnButtonStateConfirmRelease(
    ButtonState *pThis, struct BaseButton *button)
{
    if (pThis != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_PUSHED);
    }
}

PUBLIC OVERRIDE void onReleaseOnButtonStateConfirmRelease(
    ButtonState *pThis, struct BaseButton *button)
{
    if (pThis != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_RELEASED);
        notifyClickToBaseButton(button);
    }
}

PUBLIC OVERRIDE const char *toStringOnButtonStateConfirmRelease(
    ButtonState *pThis)
{
    (void)pThis;
    return "CONFIRM_RELEASE";
}

