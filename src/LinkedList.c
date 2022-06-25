#include "LinkedList.h"

PUBLIC LinkedList newLinkedList(void)
{
    LinkedList list = {
        .list = {
            .add    = (IListAddMethod)addLinkedListItem,
            .remove = (IListRemoveMethod)removeLinkedListItem,
            .find   = (IListFindMethod)findLinkedListItem
        },

        ._head = NULL
    };

    return list;
}

PUBLIC void deleteLinkedList(LinkedList * pThis)
{
    memset(pThis, 0, sizeof(LinkedList));
}

PUBLIC LinkedListItem newLinkedListItem(void)
{
    LinkedListItem item = {
        .listItem = {
            .equals = (IListItemEqualsMethod)equalsLinkedListItem
        },

        ._next = NULL
    };

    return item;
}

PUBLIC void deleteLinkedListItem(LinkedListItem * pThis)
{
    memset(pThis, 0, sizeof(LinkedListItem));
}

PUBLIC int addLinkedListItem(LinkedList * pThis, LinkedListItem * item)
{
    // 1. check parameters
    if (pThis == NULL || item == NULL) {
        return -1;
    }

    // 2. add item to head
    item->_next = pThis->_head;
    pThis->_head = item;

    return 0;
}

PUBLIC LinkedListItem * removeLinkedListItem(LinkedList * pThis, LinkedListItem * item)
{
    LinkedListItem * deleteItem = NULL;
    LinkedListItem * cursorItem = NULL;

    // 1. check parameters
    if (pThis == NULL || item == NULL) {
        return NULL;
    }

    // 2. if the item is at the head of list
    if (item->listItem.equals(&item->listItem, &pThis->_head->listItem)) {
        deleteItem = pThis->_head;

        pThis->_head = deleteItem->_next;
        deleteItem->_next = NULL;

        return deleteItem;
    }

    // 3. if the item is not at the head of list
    cursorItem = pThis->_head;

    while (cursorItem != NULL) {
        if (item->listItem.equals(&item->listItem, &cursorItem->_next->listItem)) {
            break;
        }

        cursorItem = cursorItem->_next;
    }

    if (cursorItem != NULL) {
        deleteItem = cursorItem->_next;

        cursorItem->_next = deleteItem->_next;
        deleteItem->_next = NULL;
    }

    return deleteItem;
}

PUBLIC LinkedListItem * findLinkedListItem(LinkedList * pThis, LinkedListFindCallback callback)
{
    // 1. check parameters
    if (pThis == NULL || callback == NULL) {
        return NULL;
    }

    // 2. find the item
    for (LinkedListItem * item = pThis->_head; item != NULL; item = item->_next) {
        if (callback(item)) {
            return item;
        }
    }

    return NULL;
}

PUBLIC bool equalsLinkedListItem(LinkedListItem * pThis, LinkedListItem * item)
{
    return (pThis == NULL || item == NULL) ? false : (pThis == item);
}
