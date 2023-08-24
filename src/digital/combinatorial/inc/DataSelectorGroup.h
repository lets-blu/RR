#ifndef __DATA_SELECTOR_GROUP_H__
#define __DATA_SELECTOR_GROUP_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "core/device/inc/BaseScannable.h"
#include "core/device/inc/DeviceManager.h"
#include "core/list/inc/LinkedList.h"

#include "digital/combinatorial/inc/DataSelector.h"

#include "port/common/inc/BasePin.h"

#define BaseScannable2DataSelectorGroup(instance) \
    BASE_TO_SUB_CAST(instance, DataSelectorGroup, base)

typedef struct {
    BaseScannable base;

    BasePin *_dataPin;
    BasePin *_addressPins;
    LinkedList _dataSelectors;
} DataSelectorGroup;

typedef struct {
    BasePinParameter dataPin;
    BasePinParameter addressPins;
} DataSelectorGroupParameter;

// (De)constructors
PUBLIC void constructDataSelectorGroup(
    DataSelectorGroup *instance, DataSelectorGroupParameter *parameter);

PUBLIC void deconstructDataSelectorGroup(DataSelectorGroup *instance);

// Public method(s)
PUBLIC void addSelectorToDataSelectorGroup(
    DataSelectorGroup *pThis, DataSelector *selector);

PUBLIC void removeSelectorFromDataSelectorGroup(
    DataSelectorGroup *pThis, DataSelector *selector);

PUBLIC void scanDataSelectorGroup(DataSelectorGroup *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DATA_SELECTOR_GROUP_H__

