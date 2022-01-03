#include "LinkedList.h"

PUBLIC LinkedList newLinkedList(void)
{
    LinkedList list = {
        ._count = 0,
        ._head = NULL,
        .listMethods = {
            .add = (IListAddMethod)addLinkedListItem,
            .remove = (IListRemoveMethod)removeLinkedListItem,
            .find = (IListFindMethod)findLinkedListItem
        }
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
        ._next = NULL,
        .listItemMethods = {
            .equals = (IListItemEqualsMethod)equalsLinkedListItem
        }
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

    // find the existed item
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

    return pThis->_count++;
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
    if (item->listItemMethods.equals((struct IListItem *)item, (struct IListItem *)pThis->_head))
    {
        pThis->_head = pThis->_head->_next;
        pThis->_count--;
        return;
    }

    // if item is not the head, find the previous
    previous = pThis->_head;

    while (previous != NULL)
    {
        if (item->listItemMethods.equals((struct IListItem *)item, (struct IListItem *)previous))
        {
            break;
        }

        previous = previous->_next;
    }

    // the previous is found, remove it from list
    if (previous != NULL)
    {
        previous->_next = previous->_next->_next;
        pThis->_count--;
    }
}

PUBLIC VIRTUAL LinkedListItem * findLinkedListItem(LinkedList * pThis, IListFindCallback callback)
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
        if (callback((struct IListItem *)item))
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
