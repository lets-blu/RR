#ifndef __ARRAY_LIST_TEST_H__
#define __ARRAY_LIST_TEST_H__

#include "gtest/gtest.h"
#include "inc/Utility/ArrayList.h"

#define ARRAY_LIST_TEST_MAX_ITEMS_COUNT     3
#define ARRAY_LIST_TEST_ADD_ITEM_INDEX      0
#define ARRAY_LIST_TEST_REMOVE_ITEM_INDEX   1
#define ARRAY_LIST_TEST_FIND_ITEM_INDEX     2

#define ArrayListTestItem2IListItem(item) \
    ((struct IListItem *)(item))

#define IListItem2ArrayListTestItem(item) \
    ((ArrayListTestItem *)(item))

#define isArrayListTestItem(listItem) \
    ((listItem) != NULL && \
     (listItem)->equals == (IListItemEqualsMethod)equalsArrayListTestItem)

typedef struct {
    ArrayListItem base;
    int _number;
} ArrayListTestItem;

class ArrayListTest : public ::testing::Test {
protected:
    ArrayList _arrayList;
    ArrayListTestItem _testItems[ARRAY_LIST_TEST_MAX_ITEMS_COUNT];

public:
    void SetUp();
    void TearDown();
protected:
    void GenerateArrayList();
    void CheckArrayList(int removeIndex);
};

// (De)constructors
PUBLIC ArrayListTestItem newArrayListTestItem(int number);
PUBLIC void deleteArrayListTestItem(ArrayListTestItem * pThis);

// Public methods
PUBLIC bool equalsArrayListTestItem(
    ArrayListTestItem * pThis,
    ArrayListTestItem * item);

#endif // __ARRAY_LIST_TEST_H__
