#ifndef __LINKED_LIST_TEST_H__
#define __LINKED_LIST_TEST_H__

#include "gtest/gtest.h"
#include "inc/Utility/LinkedList.h"

#define LINKED_LIST_TEST_MAX_ITEMS_COUNT    3
#define LINKED_LIST_TEST_ADD_ITEM_INDEX     0
#define LINKED_LIST_TEST_REMOVE_ITEM_INDEX  1
#define LINKED_LIST_TEST_FIND_ITEM_INDEX    2

#define LinkedListTestItem2IListItem(item) \
    ((struct IListItem *)(item))

#define IListItem2LinkedListTestItem(item) \
    ((LinkedListTestItem *)(item))

#define isLinkedListTestItem(listItem) \
    ((listItem) != NULL && \
     (listItem)->equals == (IListItemEqualsMethod)equalsLinkedListTestItem)

typedef struct {
    LinkedListItem base;
    int _number;
} LinkedListTestItem;

class LinkedListTest : public ::testing::Test {
protected:
    LinkedList _linkedList;
    LinkedListTestItem _testItems[LINKED_LIST_TEST_MAX_ITEMS_COUNT];

public:
    void SetUp();
    void TearDown();
protected:
    void GenerateLinkedList();
    void CheckLinkedList(int startIndex);
    void CheckLinkedListRemove(int removeIndex);
};

// (De)constructors
PUBLIC LinkedListTestItem newLinkedListTestItem(int number);
PUBLIC void deleteLinkedListTestItem(LinkedListTestItem * pThis);

// Public methods
PUBLIC bool equalsLinkedListTestItem(
    LinkedListTestItem * pThis,
    LinkedListTestItem * item);

#endif // __LINKED_LIST_TEST_H__
