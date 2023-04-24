#ifndef __BASE_BUTTON_H__
#define __BASE_BUTTON_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "core/log/inc/LogFilter.h"

struct IButtonState;
struct BaseButtonVtbl;

typedef struct BaseButton {
    const struct IButtonState *_currentState;
    const struct BaseButtonVtbl *vtbl;
} BaseButton;

typedef struct BaseButtonVtbl {
    void (*_notifyPush)(BaseButton *pThis);
    void (*_notifyRelease)(BaseButton *pThis);
} BaseButtonVtbl;

typedef void (*BaseButtonNotifyPushMethod)(BaseButton *);
typedef void (*BaseButtonNotifyReleaseMethod)(BaseButton *);

// Public method(s)
PUBLIC void setStateToBaseButton(
    BaseButton *pThis, const struct IButtonState *state);

PUBLIC const struct IButtonState *getStateFromBaseButton(BaseButton *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __BASE_BUTTON_H__

