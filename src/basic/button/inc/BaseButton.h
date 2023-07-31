#ifndef __BASE_BUTTON_H__
#define __BASE_BUTTON_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "core/event/inc/EventHandler.h"
#include "core/list/inc/LinkedList.h"
#include "core/log/inc/LogFilter.h"

struct ButtonState;

typedef struct BaseButton {
    const struct ButtonState *_currentState;
    LinkedList _clickHandlers;
} BaseButton;

// (De)constructors
PROTECTED void constructBaseButton(BaseButton *instance);
PROTECTED void deconstructBaseButton(BaseButton *instance);

// Public method(s)
PUBLIC void addClickHandlerToBaseButton(
    BaseButton *pThis, EventHandler *handler);

PUBLIC void removeClickHandlerFromBaseButton(
    BaseButton *pThis, EventHandler *handler);

PUBLIC void setStateToBaseButton(
    BaseButton *pThis, const struct ButtonState *state);

PUBLIC const struct ButtonState *getStateFromBaseButton(BaseButton *pThis);

PUBLIC void notifyClickToBaseButton(BaseButton *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __BASE_BUTTON_H__

