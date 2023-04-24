#ifndef __BUTTON_STATE_H__
#define __BUTTON_STATE_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"

#define IButtonState2ButtonStateReleased(instance)          \
    ((IButtonState *)((ButtonStateReleased *)(instance)))

#define IButtonState2ButtonStateConfirmPush(instance)       \
    ((IButtonState *)((ButtonStateConfirmPush *)(instance)))

#define IButtonState2ButtonStatePushed(instance)            \
    ((IButtonState *)((ButtonStatePushed *)(instance)))

#define IButtonState2ButtonStateConfirmRelease(instance)    \
    ((IButtonState *)((ButtonStateConfirmRelease *)(instance)))

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
    void (*onPush)(struct IButtonState *pThis, struct BaseButton *button);
    void (*onRelease)(struct IButtonState *pThis, struct BaseButton *button);
    const char *(*toString)(struct IButtonState *pThis);
} IButtonStateVtbl;

typedef void (*IButtonStateOnPushMethod)(IButtonState *, struct BaseButton *);
typedef void (*IButtonStateOnReleaseMethod)(IButtonState *, struct BaseButton *);
typedef const char *(*IButtonStateToStringMethod)(IButtonState *);

// Public member(s)
extern PUBLIC STATIC const IButtonState *BUTTON_STATE_RELEASED;
extern PUBLIC STATIC const IButtonState *BUTTON_STATE_CONFIRM_PUSH;
extern PUBLIC STATIC const IButtonState *BUTTON_STATE_PUSHED;
extern PUBLIC STATIC const IButtonState *BUTTON_STATE_CONFIRM_RELEASE;

// (De)constructors
PUBLIC void constructButtonStateReleased(
    ButtonStateReleased *instance);

PUBLIC void deconstrutButtonStateReleased(
    ButtonStateReleased *instance);

PUBLIC void constructButtonStateConfirmPush(
    ButtonStateConfirmPush *instance);

PUBLIC void deconstructButtonStateConfirmPush(
    ButtonStateConfirmPush *instance);

PUBLIC void constructButtonStatePushed(
    ButtonStatePushed *instance);

PUBLIC void deconstructButtonStatePushed(
    ButtonStatePushed *instance);

PUBLIC void constructButtonStateConfirmRelease(
    ButtonStateConfirmRelease *instance);

PUBLIC void deconstructButtonStateConfirmRelease(
    ButtonStateConfirmRelease *instance);

// Public method(s)
PUBLIC void onPushOnButtonStateReleased(
    ButtonStateReleased *pThis, struct BaseButton *button);

PUBLIC void onReleaseOnButtonStateReleased(
    ButtonStateReleased *pThis, struct BaseButton *button);

PUBLIC const char *toStringOnButtonStateReleased(
    ButtonStateReleased *pThis);

PUBLIC void onPushOnButtonStateConfirmPush(
    ButtonStateConfirmPush *pThis, struct BaseButton *button);

PUBLIC void onReleaseOnButtonStateConfirmPush(
    ButtonStateConfirmPush *pThis, struct BaseButton *button);

PUBLIC const char *toStringOnButtonStateConfirmPush(
    ButtonStateConfirmPush *pThis);

PUBLIC void onPushOnButtonStatePushed(
    ButtonStatePushed *pThis, struct BaseButton *button);

PUBLIC void onReleaseOnButtonStatePushed(
    ButtonStatePushed *pThis, struct BaseButton *button);

PUBLIC const char *toStringOnButtonStatePushed(
    ButtonStatePushed *pThis);

PUBLIC void onPushOnButtonStateConfirmRelease(
    ButtonStateConfirmRelease *pThis, struct BaseButton *button);

PUBLIC void onReleaseOnButtonStateConfirmRelease(
    ButtonStateConfirmRelease *pThis, struct BaseButton *button);

PUBLIC const char *toStringOnButtonStateConfirmRelease(
    ButtonStateConfirmRelease *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __BUTTON_STATE_H__

