#ifndef __ETHERNETOBJECT_H__
#define __ETHERNETOBJECT_H__

#include "stm32f10x.h"

#include "keyword.h"
#include "manual.h"
#include "strings.h"

#define NETOBJ_NUM_OF_INDEXES   	((uint8_t)4)
#define NETOBJ_NUM_OF_VALUES    	((uint8_t)1)

#define NETOBJ_TIMING_SCHEME_TYPE   ((uint8_t)0xc0)
#define NETOBJ_MODE_SETTING_TYPE    ((uint8_t)0xb7)

typedef struct EthernetObject {
	uint8_t type;
	uint8_t numOfIndexes;
	uint8_t index[NETOBJ_NUM_OF_INDEXES];
	uint8_t value[NETOBJ_NUM_OF_VALUES];
} EthernetObject;

PUBLIC STATIC void handleEthernetObject(EthernetObject object);

#endif // __ETHERNETOBJECT_H__
