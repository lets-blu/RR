#include "ArrayList.h"

PUBLIC ArrayList newArrayList(size_t arrayLength, size_t itemSize)
{
    ArrayList list = {
        .list = {
            .add    = (IListAddMethod)addArrayListItem,
            .remove = (IListRemoveMethod)removeArrayListItem,
            .find   = (IListFindMethod)findArrayListItem
        },

        ._arrayLength   = arrayLength,
        ._itemSize      = itemSize,
        ._itemsCount    = 0,
        ._base          = malloc(arrayLength * itemSize)
    };

    return list;
}

PUBLIC void deleteArrayList(ArrayList * pThis)
{
    free(pThis->_base);
    memset(pThis, 0, sizeof(ArrayList));
}

PUBLIC ArrayListItem newArrayListItem(void)
{
    ArrayListItem item = {
        .listItem = {
            .equals = (IListItemEqualsMethod)equalsArrayListItem
        }
    };

    return item;
}

PUBLIC void deleteArrayListItem(ArrayListItem * pThis)
{
    memset(pThis, 0, sizeof(ArrayListItem));
}

PUBLIC int addArrayListItem(ArrayList * pThis, ArrayListItem * item)
{
    // 1. check parameters
    if (pThis == NULL || item == NULL)
    {
        return -1;
    }

    // 2. check array status
    if (pThis->_base == NULL || pThis->_itemsCount == pThis->_arrayLength)
    {
        return -1;
    }

    // 3. add item to array
    memcpy(pThis->_base + pThis->_itemsCount * pThis->_itemSize, item, pThis->_itemSize);

    return pThis->_itemsCount++;
}

PUBLIC ArrayListItem * removeArrayListItem(ArrayList * pThis, ArrayListItem * item)
{
    int index = 0;
    uint8_t * cursor = NULL;

    // 1. check paramters
    if (pThis == NULL || item == NULL)
    {
        return NULL;
    }

    // 2. check array status
    if (pThis->_base == NULL || pThis->_itemsCount == 0)
    {
        return NULL;
    }

    // 3. find the item
    cursor = pThis->_base;

    while (cursor != pThis->_base + pThis->_itemsCount * pThis->_itemSize)
    {
        if (item->listItem.equals(&item->listItem, (struct IListItem *)cursor))
        {
            break;
        }

        index++;
        cursor += pThis->_itemSize;
    }

    // 4. remove the item
    if (cursor != pThis->_base + pThis->_itemsCount * pThis->_itemSize)
    {
        memmove(
            cursor,
            cursor + pThis->_itemSize,
            (pThis->_itemsCount - index - 1) * pThis->_itemSize);

        pThis->_itemsCount--;
        return item;
    }

    return NULL;
}

PUBLIC ArrayListItem * findArrayListItem(ArrayList * pThis, ArrayListFindCallback callback)
{
    uint8_t * cursor = NULL;

    // 1. check parameters
    if (pThis == NULL || callback == NULL)
    {
        return NULL;
    }

    // 2. check array status
    if (pThis->_base == NULL || pThis->_itemsCount == 0)
    {
        return NULL;
    }

    // 3. find the item
    cursor = pThis->_base;

    while (cursor != pThis->_base + pThis->_itemsCount * pThis->_itemSize)
    {
        if (callback((ArrayListItem *)cursor))
        {
            return (ArrayListItem *)cursor;
        }

        cursor += pThis->_itemSize;
    }

    return NULL;
}

PUBLIC bool equalsArrayListItem(ArrayListItem * pThis, ArrayListItem * item)
{
    return (pThis == NULL || item == NULL) ? false : (pThis == item);
}
