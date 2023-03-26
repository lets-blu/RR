#include "core/list/inc/LinkedList.h"

// Private method(s)
PRIVATE LinkedListItem *removeHeadFromLinkedList(LinkedList *pThis);

PRIVATE LinkedListItem *removeNonHeadFromLinkedList(
    LinkedList *pThis, LinkedListItem *previous);

// Virtual methods table
static const LinkedListItemVtbl itemVtbl = {
    .compare = (LinkedListItemCompareMethod)compareLinkedListItem
};

// Method implement(s)
PUBLIC void constructLinkedList(LinkedList *instance)
{
    if (instance != NULL) {
        instance->_itemsCount = 0;
        instance->_head = NULL;
        instance->_tail = NULL;
    }
}

PUBLIC void deconstructLinkedList(LinkedList *instance)
{
    if (instance != NULL) {
        memset(instance, 0, sizeof(LinkedList));
    }
}

PUBLIC void constructLinkedListItem(LinkedListItem *instance)
{
    if (instance != NULL) {
        instance->_next = NULL;
        instance->vtbl = &itemVtbl;
    }
}

PUBLIC void deconstructLinkedListItem(LinkedListItem *instance)
{
    if (instance != NULL) {
        memset(instance, 0, sizeof(LinkedListItem));
    }
}

PUBLIC int addItemToLinkedList(LinkedList *pThis, LinkedListItem *item)
{
    // 1. Check arguments
    if (pThis == NULL || item == NULL) {
        return -1;
    }

    // 2. Add item to list
    if (pThis->_head == NULL) {
        pThis->_head = item;
    } else {
        pThis->_tail->_next = item;
    }

    pThis->_tail = item;
    return pThis->_itemsCount++;
}

PUBLIC LinkedListItem *removeItemFromLinkedList(
    LinkedList *pThis, LinkedListItem *item)
{
    LinkedListItem *previous = NULL;

    // 1. Check arguments
    if (pThis == NULL || item == NULL) {
        return NULL;
    }

    // 2. If item at the head
    if (item->vtbl->compare(item, pThis->_head)) {
        return removeHeadFromLinkedList(pThis);
    }

    // 3. If item not at the head
    previous = pThis->_head;

    while (previous != NULL) {
        if (item->vtbl->compare(item, previous->_next)) {
            break;
        }

        previous = previous->_next;
    }

    return removeNonHeadFromLinkedList(pThis, previous);
}

PUBLIC LinkedListItem *removeIndexFromLinkedList(LinkedList *pThis, int index)
{
    LinkedListItem *previous = NULL;

    // 1. Check arguments
    if (pThis == NULL || index < 0) {
        return NULL;
    }

    // 2. If item at the head
    if (index == 0) {
        return removeHeadFromLinkedList(pThis);
    }

    // 3. If item not at the head
    previous = pThis->_head;

    for (unsigned int i = 0; i < index - 1 && previous != NULL; i++) {
        previous = previous->_next;
    }

    return removeNonHeadFromLinkedList(pThis, previous);
}

PUBLIC LinkedListItem *findItemFromLinkedList(
    LinkedList *pThis, LinkedListFindCallback callback, void *argument)
{
    // 1. Check arguments
    if (pThis == NULL || callback == NULL) {
        return NULL;
    }

    // 2. Find item
    for (LinkedListItem *item = pThis->_head; item != NULL; item = item->_next) {
        if (callback(item, argument)) {
            return item;
        }
    }

    return NULL;
}

PUBLIC bool compareLinkedListItem(LinkedListItem *pThis, LinkedListItem *item)
{
    return (pThis == NULL || item == NULL) ? false : (pThis == item);
}

PRIVATE LinkedListItem *removeHeadFromLinkedList(LinkedList *pThis)
{
    LinkedListItem *remove = NULL;

    // 1. Check pre-condition
    if (pThis->_head == NULL) {
        return NULL;
    }

    // 2. Remove head
    remove = pThis->_head;
    pThis->_head = remove->_next;
    remove->_next = NULL;

    pThis->_itemsCount--;
    return remove;
}

PRIVATE LinkedListItem *removeNonHeadFromLinkedList(
    LinkedList *pThis, LinkedListItem *previous)
{
    LinkedListItem *remove = NULL;

    // 1. Check pre-condition
    if (previous == NULL) {
        return NULL;
    }

    // 2. Remove item
    remove = previous->_next;
    previous->_next = remove->_next;
    remove->_next = NULL;

    if (pThis->_tail == remove) {
        pThis->_tail = previous;
    }

    pThis->_itemsCount--;
    return remove;
}

