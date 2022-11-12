#ifndef __ARRAY_LIST_H__
#define __ARRAY_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "utility/inc/IList.h"
#include "utility/inc/Keywords.h"

#define IS_ARRAY_LIST_CONSTRUCTED(array)    \
    ((array)->list.add != NULL &&           \
     (array)->list.remove != NULL &&        \
     (array)->list.find != NULL)

struct ArrayListItem;

typedef struct {
    struct IList list;

    size_t _arraySize;
    size_t _itemSize;

    size_t _itemsCount;
    uint8_t *_base;
} ArrayList;

typedef struct ArrayListItem {
    struct IListItem listItem;
} ArrayListItem;

typedef bool (*ArrayListFindCallback)(ArrayListItem *);

// (De)constructors
PUBLIC void constructArrayList(
    ArrayList *instance,
    size_t arraySize,
    size_t itemSize);

PUBLIC void deconstructArraList(ArrayList *instace);

PUBLIC void constructArrayListItem(ArrayListItem *instance);
PUBLIC void deconstructArrayListItem(ArrayListItem *instance);

// Public method(s)
PUBLIC int addItemToArrayList(
    ArrayList *pThis,
    ArrayListItem *item);

PUBLIC ArrayListItem *removeItemFromArrayList(
    ArrayList *pThis,
    ArrayListItem *item);

PUBLIC ArrayListItem *findItemFromArrayList(
    ArrayList *pThis,
    ArrayListFindCallback callback);

PUBLIC bool equalsBetweenArrayListItem(ArrayListItem *pThis, ArrayListItem *item);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ARRAY_LIST_H__
