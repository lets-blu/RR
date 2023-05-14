#include "basic/button/inc/BaseButton.h"
#include "basic/button/inc/ButtonState.h"

// Override method(s)
PUBLIC OVERRIDE void onPushOnButtonStateReleaseButtonState(
    IButtonState *state, struct BaseButton *button);

PUBLIC OVERRIDE void onReleaseOnButtonStateReleasedButtonState(
    IButtonState *state, struct BaseButton *button);

PUBLIC OVERRIDE const char *toStringOnButtonStateReleasedButtonState(
    IButtonState *state);

PUBLIC OVERRIDE void onPushOnButtonStateConfirmPushButtonState(
    IButtonState *state, struct BaseButton *button);

PUBLIC OVERRIDE void onReleaseOnButtonStateConfirmPushButtonState(
    IButtonState *state, struct BaseButton *button);

PUBLIC OVERRIDE const char *toStringOnButtonStateConfirmPushButtonState(
    IButtonState *state);

PUBLIC OVERRIDE void onPushOnButtonStatePushedButtonState(
    IButtonState *state, struct BaseButton *button);

PUBLIC OVERRIDE void onReleaseOnButtonStatePushedButtonState(
    IButtonState *state, struct BaseButton *button);

PUBLIC OVERRIDE const char *toStringOnButtonStatePushedButtonState(
    IButtonState *state);

PUBLIC OVERRIDE void onPushOnButtonStateConfirmReleaseButtonState(
    IButtonState *state, struct BaseButton *button);

PUBLIC OVERRIDE void onReleaseOnButtonStateConfirmReleaseButtonState(
    IButtonState *state, struct BaseButton *button);

PUBLIC OVERRIDE const char *toStringOnButtonStateConfirmReleaseButtonState(
    IButtonState *state);

// Virtual methods table
static const IButtonStateVtbl buttonStateReleasedVtbl = {
    .onPush     = onPushOnButtonStateReleaseButtonState,
    .onRelease  = onReleaseOnButtonStateReleasedButtonState,
    .toString   = toStringOnButtonStateReleasedButtonState
};

static const IButtonStateVtbl buttonStateConfirmPushVtbl = {
    .onPush     = onPushOnButtonStateConfirmPushButtonState,
    .onRelease  = onReleaseOnButtonStateConfirmPushButtonState,
    .toString   = toStringOnButtonStateConfirmPushButtonState
};

static const IButtonStateVtbl buttonStatePushedVtbl = {
    .onPush     = onPushOnButtonStatePushedButtonState,
    .onRelease  = onReleaseOnButtonStatePushedButtonState,
    .toString   = toStringOnButtonStatePushedButtonState
};

static const IButtonStateVtbl buttonStateConfirmReleaseVtbl = {
    .onPush     = onPushOnButtonStateConfirmReleaseButtonState,
    .onRelease  = onReleaseOnButtonStateConfirmReleaseButtonState,
    .toString   = toStringOnButtonStateConfirmReleaseButtonState
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
PUBLIC OVERRIDE void onPushOnButtonStateReleaseButtonState(
    IButtonState *state, struct BaseButton *button)
{
    if (state != NULL && button != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_CONFIRM_PUSH);
    }
}

PUBLIC OVERRIDE void onReleaseOnButtonStateReleasedButtonState(
    IButtonState *state, struct BaseButton *button)
{
    (void)state;
    (void)button;
}

PUBLIC OVERRIDE const char *toStringOnButtonStateReleasedButtonState(
    IButtonState *state)
{
    (void)state;
    return "RELEASED";
}

PUBLIC OVERRIDE void onPushOnButtonStateConfirmPushButtonState(
    IButtonState *state, struct BaseButton *button)
{
    if (state != NULL && button != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_PUSHED);
        button->vtbl->notifyPush(button);
    }
}

PUBLIC OVERRIDE void onReleaseOnButtonStateConfirmPushButtonState(
    IButtonState *state, struct BaseButton *button)
{
    if (state != NULL && button != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_RELEASED);
    }
}

PUBLIC OVERRIDE const char *toStringOnButtonStateConfirmPushButtonState(
    IButtonState *state)
{
    (void)state;
    return "CONFIRM_PUSH";
}

PUBLIC OVERRIDE void onPushOnButtonStatePushedButtonState(
    IButtonState *state, struct BaseButton *button)
{
    (void)state;
    (void)button;
}

PUBLIC OVERRIDE void onReleaseOnButtonStatePushedButtonState(
    IButtonState *state, struct BaseButton *button)
{
    if (state != NULL && button != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_CONFIRM_RELEASE);
    }
}

PUBLIC OVERRIDE const char *toStringOnButtonStatePushedButtonState(
    IButtonState *state)
{
    (void)state;
    return "PUSHED";
}

PUBLIC OVERRIDE void onPushOnButtonStateConfirmReleaseButtonState(
    IButtonState *state, struct BaseButton *button)
{
    if (state != NULL && button != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_PUSHED);
    }
}

PUBLIC OVERRIDE void onReleaseOnButtonStateConfirmReleaseButtonState(
    IButtonState *state, struct BaseButton *button)
{
    if (state != NULL && button != NULL) {
        setStateToBaseButton(button, BUTTON_STATE_RELEASED);
        button->vtbl->notifyRelease(button);
    }
}

PUBLIC OVERRIDE const char *toStringOnButtonStateConfirmReleaseButtonState(
    IButtonState *state)
{
    (void)state;
    return "CONFIRM_RELEASE";
}

