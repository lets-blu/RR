#include "LinkedListTest.h"

// Private methods
PRIVATE STATIC bool findLinkedListTestItemCallback(LinkedListItem * item);

TEST_F(LinkedListTest, add)
{
    // generate list
    GenerateLinkedList();
}

TEST_F(LinkedListTest, remove_Head)
{
    // generate list
    GenerateLinkedList();

    // remove the head
    LinkedListItem * removeItem = removeLinkedListItem(
        &_linkedList,
        &_testItems[0].base);

    // check result
    EXPECT_EQ(&_testItems[0].base, removeItem);

    EXPECT_EQ((size_t)LINKED_LIST_TEST_MAX_ITEMS_COUNT - 1, _linkedList._itemsCount); 
    EXPECT_EQ(&_testItems[1].base, _linkedList._head);
    EXPECT_EQ(&_testItems[LINKED_LIST_TEST_MAX_ITEMS_COUNT - 1].base, _linkedList._tail);

    // check list
    CheckLinkedList(1);
}

TEST_F(LinkedListTest, remove_Nonhead)
{
    // generate list
    GenerateLinkedList();

    // remove the non-head
    LinkedListItem * removeItem = removeLinkedListItem(
        &_linkedList,
        &_testItems[LINKED_LIST_TEST_REMOVE_ITEM_INDEX].base);

    // check result
    EXPECT_EQ(&_testItems[LINKED_LIST_TEST_REMOVE_ITEM_INDEX].base, removeItem);

    EXPECT_EQ((size_t)LINKED_LIST_TEST_MAX_ITEMS_COUNT - 1, _linkedList._itemsCount); 
    EXPECT_EQ(&_testItems[0].base, _linkedList._head);
#if (LINKED_LIST_TEST_REMOVE_ITEM_INDEX == LINKED_LIST_TEST_MAX_ITEMS_COUNT - 1)
    EXPECT_EQ(&_testItems[LINKED_LIST_TEST_MAX_ITEMS_COUNT - 2].base, _linkedList._tail);
#else
    EXPECT_EQ(&_testItems[LINKED_LIST_TEST_MAX_ITEMS_COUNT - 1].base, _linkedList._tail);
#endif

    // check list
    CheckLinkedListRemove(LINKED_LIST_TEST_REMOVE_ITEM_INDEX);
}

TEST_F(LinkedListTest, find)
{
    // generate list
    GenerateLinkedList();

    // find the item
    LinkedListItem * findItem = findLinkedListItem(
        &_linkedList,
        findLinkedListTestItemCallback);

    // check result
    EXPECT_EQ(&_testItems[LINKED_LIST_TEST_FIND_ITEM_INDEX].base, findItem);
}

TEST_F(LinkedListTest, removeAt_Head)
{
    // generate list
    GenerateLinkedList();

    // remove the head
    LinkedListItem * removeItem = removeLinkedListItemAt(&_linkedList, 0);

    // check result
    EXPECT_EQ(&_testItems[0].base, removeItem);

    EXPECT_EQ((size_t)LINKED_LIST_TEST_MAX_ITEMS_COUNT - 1, _linkedList._itemsCount);
    EXPECT_EQ(&_testItems[1].base, _linkedList._head);
    EXPECT_EQ(&_testItems[LINKED_LIST_TEST_MAX_ITEMS_COUNT - 1].base, _linkedList._tail);

    // check list
    CheckLinkedList(1);
}

TEST_F(LinkedListTest, removeAt_Nonhead)
{
    // generate list
    GenerateLinkedList();

    // remove the non-head
    LinkedListItem * removeItem = removeLinkedListItemAt(
        &_linkedList,
        LINKED_LIST_TEST_REMOVE_ITEM_INDEX);
    
    // check result
    EXPECT_EQ(&_testItems[LINKED_LIST_TEST_REMOVE_ITEM_INDEX].base, removeItem);

    EXPECT_EQ((size_t)LINKED_LIST_TEST_MAX_ITEMS_COUNT - 1, _linkedList._itemsCount);
    EXPECT_EQ(&_testItems[0].base, _linkedList._head);
#if (LINKED_LIST_TEST_REMOVE_ITEM_INDEX == LINKED_LIST_TEST_MAX_ITEMS_COUNT - 1)
    EXPECT_EQ(&_testItems[LINKED_LIST_TEST_MAX_ITEMS_COUNT- 2].base, _linkedList._tail);
#else
    EXPECT_EQ(&_testItems[LINKED_LIST_TEST_MAX_ITEMS_COUNT - 1].base, _linkedList._tail);
#endif

    // check list
    CheckLinkedListRemove(LINKED_LIST_TEST_REMOVE_ITEM_INDEX);
}

TEST_F(LinkedListTest, getItemsCount)
{
    // test in other cases
}

void LinkedListTest::SetUp()
{
    _linkedList = newLinkedList();

    for (int i = 0; i < LINKED_LIST_TEST_MAX_ITEMS_COUNT; i++)
    {
        _testItems[i] = newLinkedListTestItem(i);
    }
}

void LinkedListTest::TearDown()
{
    deleteLinkedList(&_linkedList);

    for (int i = 0; i < LINKED_LIST_TEST_MAX_ITEMS_COUNT; i++)
    {
        deleteLinkedListTestItem(&_testItems[i]);
    }
}

void LinkedListTest::GenerateLinkedList()
{
    // add items
    for (int i = 0; i < LINKED_LIST_TEST_MAX_ITEMS_COUNT; i++)
    {
        EXPECT_EQ(
            getLinkedListItemsCount(&_linkedList) - 1,
            (size_t)addLinkedListItem(&_linkedList, &_testItems[i].base));
    }

    // check result
    EXPECT_EQ((size_t)LINKED_LIST_TEST_MAX_ITEMS_COUNT, _linkedList._itemsCount);
    EXPECT_EQ(&_testItems[0].base, _linkedList._head);
    EXPECT_EQ(&_testItems[LINKED_LIST_TEST_MAX_ITEMS_COUNT - 1].base, _linkedList._tail);

    // check list
    CheckLinkedList(0);
}

void LinkedListTest::CheckLinkedList(int startIndex)
{
    for (int i = startIndex; i < LINKED_LIST_TEST_MAX_ITEMS_COUNT; i++)
    {
        if (i == LINKED_LIST_TEST_MAX_ITEMS_COUNT - 1)
        {
            EXPECT_EQ(NULL, _testItems[i].base._next);
        }
        else
        {
            EXPECT_EQ(&_testItems[i + 1].base, _testItems[i].base._next);
        }
    }
}

void LinkedListTest::CheckLinkedListRemove(int removeIndex)
{
    for (int i = 0; i < LINKED_LIST_TEST_MAX_ITEMS_COUNT; i++)
    {
        if (i == removeIndex)
        {
            continue;
        }
        else if (i == removeIndex - 1)
        {
            if (i == LINKED_LIST_TEST_MAX_ITEMS_COUNT - 2)
            {
                EXPECT_EQ(NULL, _testItems[i].base._next);
            }
            else
            {
                EXPECT_EQ(&_testItems[i + 2].base, _testItems[i].base._next);
            }
        }
        else
        {
            if (i == LINKED_LIST_TEST_MAX_ITEMS_COUNT - 1)
            {
                EXPECT_EQ(NULL, _testItems[i].base._next);
            }
            else
            {
                EXPECT_EQ(&_testItems[i + 1].base, _testItems[i].base._next);
            }
        }
    }
}

PUBLIC LinkedListTestItem newLinkedListTestItem(int number)
{
    LinkedListTestItem item = {
        .base       = newLinkedListItem(),
        ._number    = number
    };

    struct IListItem * listItem = LinkedListTestItem2IListItem(&item);
    listItem->equals = (IListItemEqualsMethod)equalsLinkedListTestItem;

    return item;
}

PUBLIC void deleteLinkedListTestItem(LinkedListTestItem * pThis)
{
    deleteLinkedListItem(&pThis->base);
    memset(pThis, 0, sizeof(LinkedListTestItem));
}

PUBLIC bool equalsLinkedListTestItem(
    LinkedListTestItem * pThis,
    LinkedListTestItem * item)
{
    if (!isLinkedListTestItem(LinkedListTestItem2IListItem(item)))
    {
        return false;
    }

    return (pThis->_number == item->_number);
}

PRIVATE STATIC bool findLinkedListTestItemCallback(LinkedListItem * item)
{
    LinkedListTestItem * testItem = (LinkedListTestItem *)item;

    if (!isLinkedListTestItem(LinkedListTestItem2IListItem(testItem)))
    {
        return false;
    }

    return (testItem->_number == LINKED_LIST_TEST_FIND_ITEM_INDEX);
}
