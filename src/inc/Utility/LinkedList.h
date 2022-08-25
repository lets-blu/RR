#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdbool.h"
#include "stdint.h"
#include "string.h"

#include "inc/Keywords.h"
#include "inc/Utility/IList.h"

struct LinkedListItem;

typedef struct LinkedList {
    struct IList list;
    size_t _itemsCount;
    struct LinkedListItem * _head;
    struct LinkedListItem * _tail;
} LinkedList;

typedef struct LinkedListItem {
    struct IListItem listItem;
    struct LinkedListItem * _next;
} LinkedListItem;

typedef bool (*LinkedListFindCallback)(LinkedListItem *);

// (De)constructors
PUBLIC LinkedList newLinkedList(void);
PUBLIC void deleteLinkedList(LinkedList * pThis);

PUBLIC LinkedListItem newLinkedListItem(void);
PUBLIC void deleteLinkedListItem(LinkedListItem * pThis);

// Public methods
PUBLIC int addLinkedListItem(LinkedList * pThis, LinkedListItem * item);
PUBLIC LinkedListItem * removeLinkedListItem(LinkedList * pThis, LinkedListItem * item);
PUBLIC LinkedListItem * findLinkedListItem(LinkedList * pThis, LinkedListFindCallback callback);

PUBLIC LinkedListItem * removeLinkedListItemAt(LinkedList * pThis, int index);
PUBLIC size_t getLinkedListItemsCount(LinkedList * pThis);

PUBLIC bool equalsLinkedListItem(LinkedListItem * pThis, LinkedListItem * item);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LINKED_LIST_H__
