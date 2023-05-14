#ifndef __BUTTON_STATE_H__
#define __BUTTON_STATE_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"

#define IButtonState2ButtonStateReleased(instance)          \
    BASE_TO_SUB_CAST(instance, ButtonStateReleased, buttonState)

#define IButtonState2ButtonStateConfirmPush(instance)       \
    BASE_TO_SUB_CAST(instance, ButtonStateConfirmPush, buttonState)

#define IButtonState2ButtonStatePushed(instance)            \
    BASE_TO_SUB_CAST(instance, ButtonStatePushed, buttonState)

#define IButtonState2ButtonStateConfirmRelease(instance)    \
    BASE_TO_SUB_CAST(instance, ButtonStateConfirmRelease, buttonState)

struct BaseButton;
struct IButtonStateVtbl;

typedef struct IButtonState {
    const struct IButtonStateVtbl *vtbl;
} IButtonState;

typedef struct {
    IButtonState buttonState;
} ButtonStateReleased;

typedef struct {
    IButtonState buttonState;
} ButtonStateConfirmPush;

typedef struct {
    IButtonState buttonState;
} ButtonStatePushed;

typedef struct {
    IButtonState buttonState;
} ButtonStateConfirmRelease;

typedef struct IButtonStateVtbl {
    void (*onPush)(IButtonState *pThis, struct BaseButton *button);
    void (*onRelease)(IButtonState *pThis, struct BaseButton *button);
    const char *(*toString)(IButtonState *pThis);
} IButtonStateVtbl;

// Public member(s)
extern PUBLIC STATIC const IButtonState *BUTTON_STATE_RELEASED;
extern PUBLIC STATIC const IButtonState *BUTTON_STATE_CONFIRM_PUSH;
extern PUBLIC STATIC const IButtonState *BUTTON_STATE_PUSHED;
extern PUBLIC STATIC const IButtonState *BUTTON_STATE_CONFIRM_RELEASE;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __BUTTON_STATE_H__

