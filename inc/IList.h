#ifndef __ILIST_H__
#define __ILIST_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdbool.h"

struct IListItem;

struct IList {
    int (*add)(struct IList * pThis, struct IListItem * item);
    struct IListItem * (*remove)(struct IList * pThis, struct IListItem * item);
    struct IListItem * (*find)(struct IList * pThis, bool (*callback)(struct IListItem *));
};

struct IListItem {
    bool (*equals)(struct IListItem * pThis, struct IListItem * item);
};

typedef bool (*IListFindCallback)(struct IListItem *);

typedef int (*IListAddMethod)(struct IList *, struct IListItem *);
typedef struct IListItem * (*IListRemoveMethod)(struct IList *, struct IListItem *);
typedef struct IListItem * (*IListFindMethod)(struct IList *, bool (*)(struct IListItem *));

typedef bool (*IListItemEqualsMethod)(struct IListItem *, struct IListItem *);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ILIST_H__
