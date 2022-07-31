#ifndef __ARRAY_LIST_H__
#define __ARRAY_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#include "IList.h"
#include "Keywords.h"

struct ArrayListItem;

typedef struct ArrayList {
    struct IList list;

    size_t _arrayLength;
    size_t _itemSize;

    size_t _itemsCount;
    uint8_t * _base;
} ArrayList;

typedef struct ArrayListItem {
    struct IListItem listItem;
} ArrayListItem;

typedef bool (*ArrayListFindCallback)(ArrayListItem *);

// (De)constructors
PUBLIC ArrayList newArrayList(size_t arrayLength, size_t itemSize);
PUBLIC void deleteArrayList(ArrayList * pThis);

PUBLIC ArrayListItem newArrayListItem(void);
PUBLIC void deleteArrayListItem(ArrayListItem * pThis);

// Public methods
PUBLIC int addArrayListItem(ArrayList * pThis, ArrayListItem * item);
PUBLIC ArrayListItem * removeArrayListItem(ArrayList * pThis, ArrayListItem * item);
PUBLIC ArrayListItem * findArrayListItem(ArrayList * pThis, ArrayListFindCallback callback);

PUBLIC bool removeArrayListItemAt(ArrayList * pThis, int index, ArrayListItem * item);
PUBLIC size_t getArrayListItemsCount(ArrayList * pThis);

PUBLIC bool equalsArrayListItem(ArrayListItem * pThis, ArrayListItem * item);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ARRAY_LIST_H__
