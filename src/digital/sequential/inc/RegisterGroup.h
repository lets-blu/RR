#ifndef __REGISTER_GROUP_H__
#define __REGISTER_GROUP_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "digital/sequential/inc/ShiftRegister.h"

#include "core/common/inc/Keywords.h"
#include "core/device/inc/BaseScannable.h"
#include "core/device/inc/DeviceManager.h"
#include "core/list/inc/LinkedList.h"
#include "core/log/inc/LogFilter.h"

#include "port/common/inc/BasePin.h"

#define BaseScannable2RegisterGroup(instance) \
    BASE_TO_SUB_CAST(instance, RegisterGroup, base)

typedef struct {
    BaseScannable base;

    BasePin *_oePin;
    BasePin *_serPin;
    BasePin *_sckPin;
    BasePin *_rckPin;

    LinkedList _shiftRegisters;
} RegisterGroup;

typedef struct {
    BasePinParameter oePin;
    BasePinParameter serPin;
    BasePinParameter sckPin;
    BasePinParameter rckPin;
} RegisterGroupParameter;

// (De)constructors
PUBLIC void constructRegisterGroup(
    RegisterGroup *instance, RegisterGroupParameter *parameter);

PUBLIC void deconstructRegisterGroup(RegisterGroup *instance);

// Public method(s)
PUBLIC void addRegisterToRegisterGroup(
    RegisterGroup *pThis, ShiftRegister *shiftRegister);

PUBLIC void removeRegisterFromRegisterGroup(
    RegisterGroup *pThis, ShiftRegister *shiftRegister);

PUBLIC void setOutputEnableToRegisterGroup(
    RegisterGroup *pThis, bool enable);

PUBLIC void outputRegisterGroup(RegisterGroup *pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __REGISTER_GROUP_H__

