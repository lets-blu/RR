#ifndef __LAMPSTATE_H__
#define __LAMPSTATE_H__

#include "stdio.h"

struct Lamp;
#include "keyword.h"
#include "watchdog.h"

struct ILampState {
    void (*turnRed)(struct Lamp * lamp);
    void (*turnGreen)(struct Lamp * lamp);
};

extern const struct ILampState LAMP_STATE_INITIAL;
extern const struct ILampState LAMP_STATE_RED;
extern const struct ILampState LAMP_STATE_YELLOW;
extern const struct ILampState LAMP_STATE_GREEN;

#endif // __LAMPSTATE_H__
