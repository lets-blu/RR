#include "inc/Utility/LinkedList.h"

// Private methods
PRIVATE LinkedListItem * removeLinkedListHead(LinkedList * pThis);
PRIVATE LinkedListItem * removeLinkedListNonhead(LinkedList * pThis, LinkedListItem * previous);

PUBLIC LinkedList newLinkedList(void)
{
    LinkedList list = {
        .list = {
            .add    = (IListAddMethod)addLinkedListItem,
            .remove = (IListRemoveMethod)removeLinkedListItem,
            .find   = (IListFindMethod)findLinkedListItem
        },

        ._itemsCount    = 0,
        ._head          = NULL,
        ._tail          = NULL
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
    if (pThis == NULL || item == NULL)
    {
        return -1;
    }

    // 2. add item to tail 
    if (pThis->_head == NULL)
    {
        pThis->_head = item;
    }
    else
    {
        pThis->_tail->_next = item;
    }

    pThis->_tail = item;
    return pThis->_itemsCount++;
}

PUBLIC LinkedListItem * removeLinkedListItem(LinkedList * pThis, LinkedListItem * item)
{
    LinkedListItem * previous = NULL;

    // 1. check parameters
    if (pThis == NULL || item == NULL)
    {
        return NULL;
    }

    // 2. if the item is at the head of list
    if (item->listItem.equals(&item->listItem, &pThis->_head->listItem))
    {
        return removeLinkedListHead(pThis);
    }

    // 3. if the item is not at the head of list
    previous = pThis->_head;

    while (previous != NULL)
    {
        if (item->listItem.equals(&item->listItem, &previous->_next->listItem))
        {
            break;
        }

        previous = previous->_next;
    }

    return removeLinkedListNonhead(pThis, previous);
}

PUBLIC LinkedListItem * findLinkedListItem(LinkedList * pThis, LinkedListFindCallback callback)
{
    // 1. check parameters
    if (pThis == NULL || callback == NULL)
    {
        return NULL;
    }

    // 2. find the item
    for (LinkedListItem * item = pThis->_head; item != NULL; item = item->_next)
    {
        if (callback(item))
        {
            return item;
        }
    }

    return NULL;
}

PUBLIC LinkedListItem * removeLinkedListItemAt(LinkedList * pThis, int index)
{
    LinkedListItem * previous = NULL;

    // 1. check parameters
    if (pThis == NULL || index < 0)
    {
        return NULL;
    }

    // 2. if the item is at the head of the list
    if (index == 0)
    {
        return removeLinkedListHead(pThis);
    }

    // 3. if the item is not at the head of the list
    previous = pThis->_head;

    for (int i = 0; i < index - 1 && previous != NULL; i++)
    {
        previous = previous->_next;
    }

    return removeLinkedListNonhead(pThis, previous);
}

PUBLIC size_t getLinkedListItemsCount(LinkedList * pThis)
{
    return (pThis == NULL) ? 0 : pThis->_itemsCount;
}

PUBLIC bool equalsLinkedListItem(LinkedListItem * pThis, LinkedListItem * item)
{
    return (pThis == NULL || item == NULL) ? false : (pThis == item);
}

PRIVATE LinkedListItem * removeLinkedListHead(LinkedList * pThis)
{
    LinkedListItem * removeItem = NULL;

    if (pThis->_head == NULL)
    {
        return NULL;
    }

    removeItem = pThis->_head;
    pThis->_head = removeItem->_next;
    removeItem->_next = NULL;

    if (pThis->_head == NULL)
    {
        pThis->_tail = NULL;
    }

    pThis->_itemsCount--;
    return removeItem;
}

PRIVATE LinkedListItem * removeLinkedListNonhead(LinkedList * pThis, LinkedListItem * previous)
{
    LinkedListItem * removeItem = NULL;

    if (previous == NULL)
    {
        return NULL;
    }

    removeItem = previous->_next;
    previous->_next = removeItem->_next;
    removeItem->_next = NULL;

    if (pThis->_tail == removeItem)
    {
        pThis->_tail = previous;
    }

    pThis->_itemsCount--;
    return removeItem;
}
