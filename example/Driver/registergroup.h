#ifndef __REGISTERGROUP_H__
#define __REGISTERGROUP_H__

#include "stdbool.h"
#include "stdlib.h"
#include "stm32f10x.h"

#include "gpiopin.h"
#include "keyword.h"
#include "shiftregister.h"

#define DEFAULT_REGISTER_GROUP  (&registerGroup1)

typedef struct RegisterGroup {
    ShiftRegister * _head;
    uint8_t _numOfRegisters;
	
    GPIOPin _serPin, _sckPin, _rckPin, _ePin;
} RegisterGroup;

extern PUBLIC STATIC RegisterGroup registerGroup1;
PUBLIC void staticRegisterGroup(void);

PUBLIC void setRegisterGroupBit(RegisterGroup * this, uint8_t bit);
PUBLIC void resetRegisterGroupBit(RegisterGroup * this, uint8_t bit);
PUBLIC void outputRegisterGroup(RegisterGroup * this);

#endif // __REGISTERGROUP_H__
