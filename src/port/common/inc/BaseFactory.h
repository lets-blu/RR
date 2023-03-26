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

typedef struct BaseFactoryVtbl {
    void (*_doCreateBasePin)(
        BaseFactory *pThis, BasePin *instance, void *port, unsigned int pin);

    void (*_doDestoryBasePin)(BaseFactory *pThis, BasePin *instance);
} BaseFactoryVtbl;

typedef void (*BaseFactoryDoCreateBasePinMethod)(
    BaseFactory *, BasePin *, void *, unsigned int);

typedef void (*BaseFactoryDoDestoryBasePinMethod)(BaseFactory *, BasePin *);

// Public method(s)
PUBLIC void createBasePinByBaseFactory(
    BaseFactory *pThis, BasePin *instance, void *port, unsigned int pin);

PUBLIC void destoryBasePinByBaseFactory(BaseFactory *pThis, BasePin *instance);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __BASE_FACTORY_H__

