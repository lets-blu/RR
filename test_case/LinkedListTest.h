#ifndef __LINKED_LIST_TEST_H__
#define __LINKED_LIST_TEST_H__

#include "gtest/gtest.h"
#include "LinkedList.h"

#define LINKED_LIST_TEST_MAX_ITEMS_COUNT    3
#define LINKED_LIST_TEST_ADD_ITEM_INDEX     0
#define LINKED_LIST_TEST_REMOVE_ITEM_INDEX  1
#define LINKED_LIST_TEST_FIND_ITEM_INDEX    2

#define IS_LINKED_LIST_TEST_ITEM(listItem) \
    ((listItem) != NULL && \
     (listItem)->equals == (ListItemEqualsMethod)equalsLinkedListTestItem)

typedef struct {
    LinkedListItem base;
    int _number;
} LinkedListTestItem;

class LinkedListTest : public ::testing::Test {
public:
    static int findIndex;
protected:
    LinkedList _linkedList;
    LinkedListTestItem _testItems[LINKED_LIST_TEST_MAX_ITEMS_COUNT];

public:
    void SetUp();
    void TearDown();
protected:
    void GenerateLinkedList();
    int GetLinkedListItemCount();
};

// (De)constructors
PUBLIC LinkedListTestItem newLinkedListTestItem(int number);
PUBLIC void deleteLinkedListTestItem(LinkedListTestItem * pThis);

// Public methods
PUBLIC VIRTUAL bool equalsLinkedListTestItem(
        LinkedListTestItem * pThis,
        LinkedListTestItem * item);

PUBLIC STATIC bool findLinkedListTestItemCallback(LinkedListTestItem * item);

#endif // __LINKED_LIST_TEST_H__
