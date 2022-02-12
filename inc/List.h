#ifndef __LIST_H__
#define __LIST_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

#define IS_LIST_CREATED(list) \
    ((list)->add != NULL && (list)->remove != NULL && (list)->find != NULL)

struct ListItem;

struct List {
    int32_t (*add)(struct List * pThis, struct ListItem * item);
    void (*remove)(struct List * pThis, struct ListItem * item);
    struct ListItem * (*find)(struct List * pThis, bool (*callback)(struct ListItem *));
};

struct ListItem {
    bool (*equals)(struct ListItem * pThis, struct ListItem * item);
};

typedef bool (*ListFindCallback)(struct ListItem *);

typedef int32_t (*ListAddMethod)(struct List *, struct ListItem *);
typedef void (*ListRemoveMethod)(struct List *, struct ListItem *);
typedef struct ListItem * (*ListFindMethod)(struct List *, bool (*)(struct ListItem *));

typedef bool (*ListItemEqualsMethod)(struct ListItem *, struct ListItem *);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LIST_H__
