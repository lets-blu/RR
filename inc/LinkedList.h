#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

#include "Keywords.h"
#include "List.h"

#define IS_LINKED_LIST_CREATED(linkedList) \
    (IS_LIST_CREATED(&(linkedList)->base))

struct LinkedListItem;

typedef struct {
    struct List base;
    struct LinkedListItem * _head;
} LinkedList;

typedef struct LinkedListItem {
    struct ListItem base;
    struct LinkedListItem * _next;
} LinkedListItem;

typedef bool (*LinkedListFindCallback)(LinkedListItem *);

// (De)constructors
PUBLIC LinkedList newLinkedList(void);
PUBLIC void deleteLinkedList(LinkedList * pThis);

PUBLIC LinkedListItem newLinkedListItem(void);
PUBLIC void deleteLinkedListItem(LinkedListItem * pThis);

// Public methods
PUBLIC VIRTUAL int32_t addLinkedListItem(LinkedList * pThis, LinkedListItem * item);
PUBLIC VIRTUAL void removeLinkedListItem(LinkedList * pThis, LinkedListItem * item);

PUBLIC VIRTUAL LinkedListItem * findLinkedListItem(
        LinkedList * pThis,
        LinkedListFindCallback callback);

PUBLIC VIRTUAL bool equalsLinkedListItem(LinkedListItem * pThis, LinkedListItem * item);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LINKED_LIST_H__
