#include "LinkedList.h"

PUBLIC LinkedList newLinkedList(void)
{
    LinkedList list = {
        .base = {
            .add = (ListAddMethod)addLinkedListItem,
            .remove = (ListRemoveMethod)removeLinkedListItem,
            .find = (ListFindMethod)findLinkedListItem
        },
        ._head = NULL
    };

    return list;
}

PUBLIC void deleteLinkedList(LinkedList * pThis)
{
    (void)pThis;
}

PUBLIC LinkedListItem newLinkedListItem(void)
{
    LinkedListItem item = {
        .base = {
            .equals = (ListItemEqualsMethod)equalsLinkedListItem
        },
        ._next = NULL
    };

    return item;
}

PUBLIC void deleteLinkedListItem(LinkedListItem * pThis)
{
    (void)pThis;
}

PUBLIC VIRTUAL int32_t addLinkedListItem(LinkedList * pThis, LinkedListItem * item)
{
    int32_t index = -1;
    LinkedListItem * existed = NULL;

    // check parameters
    if (pThis == NULL || item == NULL)
    {
        return -1;
    }

    // find the existed
    index = 0;
    existed = pThis->_head;

    while (existed != NULL)
    {
        if (item == existed)
        {
            return index;
        }

        index++;
        existed = existed->_next;
    }

    // add item to list
    item->_next = pThis->_head;
    pThis->_head = item;

    return 0;
}

PUBLIC VIRTUAL void removeLinkedListItem(LinkedList * pThis, LinkedListItem * item)
{
    LinkedListItem * previous = NULL;

    // check parameters
    if (pThis == NULL || item == NULL)
    {
        return;
    }

    // if item is the head
    if (item->base.equals(&item->base, &pThis->_head->base))
    {
        pThis->_head = pThis->_head->_next;
        return;
    }

    // if item is not the head, find the previous
    previous = pThis->_head;

    while (previous != NULL)
    {
        if (item->base.equals(&item->base, &previous->_next->base))
        {
            break;
        }

        previous = previous->_next;
    }

    // if the previous is found, remove it from list
    if (previous != NULL)
    {
        previous->_next = previous->_next->_next;
    }
}

PUBLIC VIRTUAL LinkedListItem * findLinkedListItem(
        LinkedList * pThis,
        LinkedListFindCallback callback)
{
    LinkedListItem * item = NULL;

    // check parameters
    if (pThis == NULL || callback == NULL)
    {
        return NULL;
    }

    // find item
    item = pThis->_head;

    while (item != NULL)
    {
        if (callback(item))
        {
            return item;
        }

        item = item->_next;
    }

    return NULL;
}

PUBLIC VIRTUAL bool equalsLinkedListItem(LinkedListItem * pThis, LinkedListItem * item)
{
    return (pThis == NULL || item == NULL) ? false : (pThis == item);
}
