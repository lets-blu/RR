#ifndef __ARRAY_LIST_TEST_H__
#define __ARRAY_LIST_TEST_H__

#include "ArrayList.h"
#include "gtest/gtest.h"

#define ARRAY_LIST_TEST_MAX_ITEM_COUNT		3
#define ARRAY_LIST_TEST_ADD_ITEM_INDEX		0
#define ARRAY_LIST_TEST_REMOVE_ITEM_INDEX	1
#define ARRAY_LIST_TEST_FIND_ITEM_INDEX		2

#define IS_ARRAY_NUMBER_ITEM(item) \
    ((item) != NULL && \
     ((struct IListItem *)(item))->equals == (IListItemEqualsMethod)equalsArrayNumberItem)

typedef struct {
    ArrayListItem base;
    int _number;
} ArrayNumberItem;

class ArrayListTest : public ::testing::Test {
protected:
    ArrayList _arrayList;
    ArrayNumberItem _numberItems[ARRAY_LIST_TEST_MAX_ITEM_COUNT];

public:
    void SetUp();
    void TearDown();
protected:
    void GenerateArrayList();
    int GetArrayListItemCount();
};

// (De)constructors
PUBLIC ArrayNumberItem newArrayNumberItem(int number);
PUBLIC void deleteArrayNumberItem(ArrayNumberItem * pThis);

// Public reload methods
PUBLIC bool equalsArrayNumberItem(ArrayNumberItem * pThis, ArrayNumberItem * item);

#endif // __ARRAY_LIST_TEST_H__
