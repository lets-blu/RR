#include "basic/button/inc/BaseButton.h"
#include "basic/button/inc/ButtonState.h"

// Private member(s)
PRIVATE STATIC LogFilter filter
    = STATIC_LOG_FILTER("BaseButton", LOG_LEVEL_INFO);

// Protected method(s)
PROTECTED void constructBaseButton(BaseButton *instance);
PROTECTED void deconstructBaseButton(BaseButton *instance);

// Method implement(s)
PROTECTED void constructBaseButton(BaseButton *instance)
{
    if (instance != NULL) {
        instance->_currentState = BUTTON_STATE_RELEASED;
        instance->vtbl = NULL;
    }
}

PROTECTED void deconstructBaseButton(BaseButton *instance)
{
    if (instance != NULL) {
        memset(instance, 0, sizeof(BaseButton));
    }
}

PUBLIC void setStateToBaseButton(
    BaseButton *pThis, const struct IButtonState *state)
{
    const IButtonState *previousState = NULL;

    if (pThis == NULL || state == NULL) {
        return;
    }

    previousState = pThis->_currentState;
    pThis->_currentState = state;

    LOG_I(&filter, "0x%x setState, %s -> %s",
        pThis,
        previousState->vtbl->toString((IButtonState *)previousState),
        state->vtbl->toString((IButtonState *)state));
}

PUBLIC const struct IButtonState *getStateFromBaseButton(BaseButton *pThis)
{
    return (pThis == NULL) ? NULL : pThis->_currentState;
}

