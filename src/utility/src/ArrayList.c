#include "utility/inc/ArrayList.h"

// Private method(s)
PRIVATE bool removeIndexFromArrayList(
    ArrayList *pThis,
    int index,
    ArrayListItem* buffer);

PUBLIC void constructArrayList(
    ArrayList *instance,
    size_t arraySize,
    size_t itemSize)
{
    instance->list.add = (IListAddMethod)addItemToArrayList;
    instance->list.remove = (IListRemoveMethod)removeItemFromArrayList;
    instance->list.find = (IListFindMethod)findItemFromArrayList;

    instance->_arraySize = arraySize;
    instance->_itemSize = itemSize;

    instance->_itemsCount = 0;
    instance->_base = malloc(arraySize * itemSize);
}

PUBLIC void deconstructArrayList(ArrayList *instance)
{
    free(instance->_base);
    memset(instance, 0, sizeof(ArrayList));
}

PUBLIC void constructArrayListItem(ArrayListItem *instance)
{
    instance->listItem.equals = (IListItemEqualsMethod)equalsBetweenArrayListItem;
}

PUBLIC void deconstructArrayListItem(ArrayListItem *instance)
{
    memset(instance, 0, sizeof(ArrayListItem));
}

PUBLIC int addItemToArrayList(
    ArrayList *pThis,
    ArrayListItem *item)
{
    // 1. Check parameters
    if (pThis == NULL || item == NULL)
    {
        return -1;
    }

    // 2. Check array status
    if (pThis->_base == NULL || pThis->_itemsCount == pThis->_arraySize)
    {
        return -1;
    }

    // 3. Copy item to the list
    memcpy(
        pThis->_base + pThis->_itemsCount * pThis->_itemSize,
        item,
        pThis->_itemSize);

    return pThis->_itemsCount++;
}

PUBLIC ArrayListItem *removeItemFromArrayList(
    ArrayList *pThis,
    ArrayListItem *item)
{
    int index = 0;
    uint8_t *cursor = NULL;

    // 1. Check parameters
    if (pThis == NULL || item == NULL)
    {
        return NULL;
    }

    // 2. Check array status
    if (pThis->_base == NULL || pThis->_itemsCount == 0)
    {
        return NULL;
    }

    // 3. Find item in the list
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

    return removeIndexFromArrayList(pThis, index, NULL) ? item : NULL;
}

PUBLIC ArrayListItem *findItemFromArrayList(
    ArrayList *pThis,
    ArrayListFindCallback callback)
{
    uint8_t *cursor = NULL;

    // 1. Check parameters
    if (pThis == NULL || callback == NULL)
    {
        return NULL;
    }

    // 2. Check array status
    if (pThis->_base == NULL || pThis->_itemsCount == 0)
    {
        return NULL;
    }

    // 3. Find item in the list
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

PRIVATE bool removeIndexFromArrayList(
    ArrayList *pThis,
    int index,
    ArrayListItem *buffer)
{
    // 1. Check parameters
    if (pThis == NULL || index < 0)
    {
        return NULL;
    }

    // 2. Check array status
    if (pThis->_base == NULL || (size_t)index >= pThis->_itemsCount)
    {
        return NULL;
    }

    // 3. Copy item in the list
    if (buffer != NULL)
    {
        memcpy(buffer, pThis->_base + index * pThis->_itemSize, pThis->_itemSize);
    }

    memmove(
        pThis->_base + index * pThis->_itemSize,
        pThis->_base + (index + 1) * pThis->_itemSize,
        (pThis->_itemsCount - index - 1) * pThis->_itemSize);
    
    pThis->_itemsCount--;

    return true;
}

PUBLIC bool equalsBetweenArrayListItem(ArrayListItem *pThis, ArrayListItem *item)
{
    return (pThis == NULL || item == NULL) ? false : (pThis == item);
}
