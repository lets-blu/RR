#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

#include "IList.h"
#include "Keywords.h"

#define IS_LINKED_LIST_CREATED(linkedList) \
    (IS_ILIST_CREATED(&(linkedList)->listMethods))

struct LinkedListItem;

typedef struct {
    uint32_t _count;
    struct LinkedListItem * _head;
    struct IList listMethods;
} LinkedList;

typedef struct LinkedListItem {
    struct LinkedListItem * _next;
    struct IListItem listItemMethods;
} LinkedListItem;

// (De)constructors
PUBLIC LinkedList newLinkedList(void);
PUBLIC void deleteLinkedList(LinkedList * pThis);

PUBLIC LinkedListItem newLinkedListItem(void);
PUBLIC void deleteLinkedListItem(LinkedListItem * pThis);

// Public methods
PUBLIC VIRTUAL int32_t addLinkedListItem(LinkedList * pThis, LinkedListItem * item);
PUBLIC VIRTUAL void removeLinkedListItem(LinkedList * pThis, LinkedListItem * item);
PUBLIC VIRTUAL LinkedListItem * findLinkedListItem(LinkedList * pThis, IListFindCallback callback);

PUBLIC VIRTUAL bool equalsLinkedListItem(LinkedListItem * pThis, LinkedListItem * item);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LINKED_LIST_H__
