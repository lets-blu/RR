#ifndef __ILIST_H__
#define __ILIST_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

#define IS_ILIST_CREATED(list) \
    ((list)->add != NULL && (list)->remove != NULL && (list)->find != NULL)

struct IListItem;
typedef bool (*IListFindCallback)(struct IListItem *);

struct IList {
    int32_t (*add)(struct IList * pThis, struct IListItem * item);
    void (*remove)(struct IList * pThis, struct IListItem * item);
    struct IListItem * (*find)(struct IList * pThis, IListFindCallback callback);
};

struct IListItem {
    bool (*equals)(struct IListItem * pThis, struct IListItem * item);
};

typedef int32_t (*IListAddMethod)(struct IList *, struct IListItem *);
typedef void (*IListRemoveMethod)(struct IList *, struct IListItem *);
typedef struct IListItem * (*IListFindMethod)(struct IList *, IListFindCallback);

typedef bool (*IListItemEqualsMethod)(struct IListItem *, struct IListItem *);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ILIST_H__
