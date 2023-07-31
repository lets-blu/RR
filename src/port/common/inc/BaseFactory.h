#ifndef __BASE_FACTORY_H__
#define __BASE_FACTORY_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "port/common/inc/BasePin.h"

struct BaseFactoryVtbl;

typedef struct {
    const struct BaseFactoryVtbl *vtbl;
} BaseFactory;

typedef enum {
    BASE_FACTORY_DIGITAL_PIN,
    BASE_FACTORY_ANALOG_PIN,
    BASE_FACTORY_ADDRESS_PIN
} BaseFactoryPinType;

typedef struct BaseFactoryVtbl {
    void (*_doCreatePin)(
        BaseFactory *pThis,
        BaseFactoryPinType type,
        BasePin *instance,
        BasePinParameter *parameter);

    void (*_doDestoryPin)(
        BaseFactory *pThis, BaseFactoryPinType type, BasePin *instance);

    unsigned int (*_doGetPinSize)(BaseFactory *pThis);

    // TODO: need to remove
    void (*_doCreateBasePin)(
        BaseFactory *pThis, BasePin *instance, void *port, unsigned int pin);

    // TODO: need to remove
    void (*_doDestoryBasePin)(BaseFactory *pThis, BasePin *instance);
} BaseFactoryVtbl;

// (De)constructors
PROTECTED void constructBaseFactory(BaseFactory *instance);
PROTECTED void deconstructBaseFactory(BaseFactory *instance);

// Public method(s)
PUBLIC void createPinByBaseFactory(
    BaseFactory *pThis,
    BaseFactoryPinType type,
    BasePin *instance,
    BasePinParameter *parameter);

PUBLIC void destoryPinByBaseFactory(
    BaseFactory *pThis, BaseFactoryPinType type, BasePin *instance);

PUBLIC unsigned int getPinSizeFromBaseFactory(BaseFactory *pThis);

// TODO: need to remove
PUBLIC void createBasePinByBaseFactory(
    BaseFactory *pThis, BasePin *instance, void *port, unsigned int pin);

// TODO: need to remove
PUBLIC void destoryBasePinByBaseFactory(BaseFactory *pThis, BasePin *instance);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __BASE_FACTORY_H__

