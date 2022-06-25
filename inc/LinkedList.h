#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdbool.h"
#include "stddef.h"
#include "string.h"

#include "IList.h"
#include "Keywords.h"

struct LinkedListItem;

typedef struct LinkedList {
    struct IList list;
    struct LinkedListItem * _head;
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

// Public reload methods
PUBLIC int addLinkedListItem(LinkedList * pThis, LinkedListItem * item);
PUBLIC LinkedListItem * removeLinkedListItem(LinkedList * pThis, LinkedListItem * item);
PUBLIC LinkedListItem * findLinkedListItem(LinkedList * pThis, LinkedListFindCallback callback);

PUBLIC bool equalsLinkedListItem(LinkedListItem * pThis, LinkedListItem * item);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LINKED_LIST_H__
