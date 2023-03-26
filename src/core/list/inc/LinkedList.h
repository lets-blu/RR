#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "core/common/inc/Keywords.h"

struct LinkedListItem;
struct LinkedListItemVtbl;

typedef struct {
    unsigned int _itemsCount;
    struct LinkedListItem *_head;
    struct LinkedListItem *_tail;
} LinkedList;

typedef struct LinkedListItem {
    struct LinkedListItem *_next;
    const struct LinkedListItemVtbl *vtbl;
} LinkedListItem;

typedef struct LinkedListItemVtbl {
    bool (*compare)(LinkedListItem *pThis, LinkedListItem *item);
} LinkedListItemVtbl;

typedef bool (*LinkedListFindCallback)(LinkedListItem *, void *);
typedef bool (*LinkedListItemCompareMethod)(LinkedListItem *, LinkedListItem *);

// (De)constructors
PUBLIC void constructLinkedList(LinkedList *instance);
PUBLIC void deconstructLinkedList(LinkedList *instance);

PUBLIC void constructLinkedListItem(LinkedListItem *instance);
PUBLIC void deconstructLinkedListItem(LinkedListItem *instance);

// Public method(s)
PUBLIC int addItemToLinkedList(LinkedList *pThis, LinkedListItem *item);

PUBLIC LinkedListItem *removeItemFromLinkedList(
    LinkedList *pThis, LinkedListItem *item);

PUBLIC LinkedListItem *removeIndexFromLinkedList(LinkedList *pThis, int index);

PUBLIC LinkedListItem *findItemFromLinkedList(
    LinkedList *pThis, LinkedListFindCallback callback, void *argument);

PUBLIC bool compareLinkedListItem(LinkedListItem *pThis, LinkedListItem *item);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LINKED_LIST_H__

