#ifndef __LINKED_LIST_TEST_H__
#define __LINKED_LIST_TEST_H__

#include "LinkedList.h"
#include "gtest/gtest.h"

#define LINKED_LIST_TEST_MAX_ITEM_COUNT		3
#define LINKED_LIST_TEST_ADD_ITEM_INDEX		0
#define LINKED_LIST_TEST_REMOVE_ITEM_INDEX	1
#define LINKED_LIST_TEST_FIND_ITEM_INDEX	2

#define IS_LINKED_NUMBER_ITEM(item)	\
    ((item) != NULL && \
     ((struct IListItem *)(item))->equals == (IListItemEqualsMethod)equalsLinkedNumberItem)

typedef struct {
    LinkedListItem base;
    int _number;
} LinkedNumberItem;

class LinkedListTest : public ::testing::Test {
public:
    static int findIndex;
protected:
    LinkedList _linkedList;
    LinkedNumberItem _numberItems[LINKED_LIST_TEST_MAX_ITEM_COUNT];

public:
    void SetUp();
    void TearDown();
protected:
    void GenerateLinkedList();
    int GetLinkedListItemCount();
};

// (De)constructors
PUBLIC LinkedNumberItem newLinkedNumberItem(int number);
PUBLIC void deleteLinkedNumberItem(LinkedNumberItem * pThis);

// Public reload methods
PUBLIC bool equalsLinkedNumberItem(LinkedNumberItem * pThis, LinkedNumberItem * item);

#endif // __LINKED_LIST_TEST_H__
