#ifndef __BASE_SCANNABLE_H__
#define __BASE_SCANNABLE_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"
#include "core/list/inc/LinkedList.h"

struct BaseScannableVtbl;

typedef struct BaseScannable {
    LinkedListItem base;
    const struct BaseScannableVtbl *vtbl;
} BaseScannable;

typedef struct BaseScannableVtbl {
    void (*scan)(BaseScannable *pThis);
} BaseScannableVtbl;

typedef void (*BaseScannableScanMethod)(BaseScannable *);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __BASE_SCANNABLE_H__
