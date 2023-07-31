#ifndef __BUTTON_STATE_H__
#define __BUTTON_STATE_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"

struct BaseButton;
struct ButtonStateVtbl;

typedef struct ButtonState {
    const struct ButtonStateVtbl *vtbl;
} ButtonState;

typedef struct ButtonStateVtbl {
    void (*onPush)(ButtonState *pThis, struct BaseButton *button);
    void (*onRelease)(ButtonState *pThis, struct BaseButton *button);
    const char *(*toString)(ButtonState *pThis);
} ButtonStateVtbl;

// Public member(s)
extern PUBLIC STATIC const ButtonState *BUTTON_STATE_RELEASED;
extern PUBLIC STATIC const ButtonState *BUTTON_STATE_CONFIRM_PUSH;
extern PUBLIC STATIC const ButtonState *BUTTON_STATE_PUSHED;
extern PUBLIC STATIC const ButtonState *BUTTON_STATE_CONFIRM_RELEASE;

// Public method(s)
PUBLIC void onPushOnButtonState(
    ButtonState *pThis, struct BaseButton *button);

PUBLIC void onReleaseOnButtonState(
    ButtonState *pThis, struct BaseButton *button);

PUBLIC const char *toStringOnButtonState(ButtonState *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __BUTTON_STATE_H__

