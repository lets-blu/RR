#ifndef __IMANUALSTATE_H__
#define __IMANUALSTATE_H__

#include "FreeRTOS.h"
#include "queue.h"

struct Manual;
#include "keyword.h"
#include "strings.h"

struct IManualState {
	const char * (*getName)(struct Manual * manual);
    void (*onLampButtonClick)(struct Manual * manual);
    void (*onRoadButtonClick)(struct Manual * manual);
    void (*onOKButtonClick)(struct Manual * manual);
    void (*onQuitButtonClick)(struct Manual * manual);
};

extern const struct IManualState MANUAL_STATE_INITIAL;
extern const struct IManualState MANUAL_STATE_ALL_RED;
extern const struct IManualState MANUAL_STATE_EASTWEST_LEFTTURN;
extern const struct IManualState MANUAL_STATE_EASTWEST_STRAIGHT;
extern const struct IManualState MANUAL_STATE_SOUTHNORTH_LEFTTURN;
extern const struct IManualState MANUAL_STATE_SOUTHNORTH_STRAIGHT;

#endif // __IMANUALSTATE_H__
