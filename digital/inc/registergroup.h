#ifndef __REGISTERGROUP_H__
#define __REGISTERGROUP_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

#include "gpiopin.h"
#include "keywords.h"
#include "shiftregister.h"

typedef struct {
    GPIOPin oePin;
    GPIOPin serPin, sckPin, rckPin;
} RegisterGroupPins;

typedef struct {
    RegisterGroupPins _pins;

    ShiftRegister * _registers;
    uint8_t _registersCount;
} RegisterGroup;

// Constructor
PUBLIC RegisterGroup newRegisterGroup(RegisterGroupPins pins);

// Public method(s)
PUBLIC void addRegisterToGroup(RegisterGroup * pThis, ShiftRegister * reg);

PUBLIC void setRegisterGroupBit(RegisterGroup * pThis, uint8_t bit);
PUBLIC void resetRegisterGroupBit(RegisterGroup * pThis, uint8_t bit);

PUBLIC void setRegisterGroupOutputEnabled(RegisterGroup * pThis, bool enabled);
PUBLIC void outputRegisterGroup(RegisterGroup * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __REGISTERGROUP_H__
