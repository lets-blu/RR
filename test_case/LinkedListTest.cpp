#include "LinkedListTest.h"

// Static members
int LinkedListTest::findIndex = -1;

TEST_F(LinkedListTest, add)
{
    // generate list
    GenerateLinkedList();
}

TEST_F(LinkedListTest, addExisted)
{
    // generate list
    GenerateLinkedList();

    // add existed item
    int32_t index = addLinkedListItem(
            &_linkedList,
            &_testItems[LINKED_LIST_TEST_ADD_ITEM_INDEX].base);

    EXPECT_EQ(LINKED_LIST_TEST_ADD_ITEM_INDEX, index);

    // check result
    EXPECT_EQ(&_testItems[0].base, _linkedList._head);
    EXPECT_EQ(LINKED_LIST_TEST_MAX_ITEMS_COUNT, GetLinkedListItemCount());

    // check list
    for (int i = 0; i < LINKED_LIST_TEST_MAX_ITEMS_COUNT; i++)
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

TEST_F(LinkedListTest, removeHead)
{
    // generate list
    GenerateLinkedList();

    // remove head item
    removeLinkedListItem(&_linkedList, &_testItems[0].base);

    // check result
    EXPECT_EQ(&_testItems[1].base, _linkedList._head);
    EXPECT_EQ(LINKED_LIST_TEST_MAX_ITEMS_COUNT - 1, GetLinkedListItemCount());

    // check list
    for (int i = 1; i < LINKED_LIST_TEST_MAX_ITEMS_COUNT; i++)
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

TEST_F(LinkedListTest, removeNonhead)
{
    // generate list
    GenerateLinkedList();

    // remove non-head item
    removeLinkedListItem(
            &_linkedList,
            &_testItems[LINKED_LIST_TEST_REMOVE_ITEM_INDEX].base);

    // check result
    EXPECT_EQ(&_testItems[0].base, _linkedList._head);
    EXPECT_EQ(LINKED_LIST_TEST_MAX_ITEMS_COUNT - 1, GetLinkedListItemCount());

    // check list
    for (int i = 0; i < LINKED_LIST_TEST_MAX_ITEMS_COUNT; i++)
    {
        if (i == LINKED_LIST_TEST_REMOVE_ITEM_INDEX)
        {
            continue;
        }
        else if (i == LINKED_LIST_TEST_REMOVE_ITEM_INDEX - 1)
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

TEST_F(LinkedListTest, find)
{
    // generate list
    GenerateLinkedList();

    // find existed item
    findIndex = LINKED_LIST_TEST_FIND_ITEM_INDEX;

    LinkedListItem *item = findLinkedListItem(
            &_linkedList,
            (LinkedListFindCallback)findLinkedListTestItemCallback);

    EXPECT_EQ(&_testItems[LINKED_LIST_TEST_FIND_ITEM_INDEX].base, item);
}

TEST_F(LinkedListTest, findNonexist)
{
    // generate list
    GenerateLinkedList();

    // find non-exist item
    findIndex = LINKED_LIST_TEST_MAX_ITEMS_COUNT;

    LinkedListItem *item = findLinkedListItem(
            &_linkedList,
            (LinkedListFindCallback)findLinkedListTestItemCallback);

    EXPECT_EQ(NULL, item);
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
    // add item to list
    for (int i = LINKED_LIST_TEST_MAX_ITEMS_COUNT; i > 0; i--)
    {
        EXPECT_EQ(0, addLinkedListItem(&_linkedList, &_testItems[i - 1].base));
    }

    // check result
    EXPECT_EQ(&_testItems[0].base, _linkedList._head);
    EXPECT_EQ(LINKED_LIST_TEST_MAX_ITEMS_COUNT, GetLinkedListItemCount());

    // check list
    for (int i = 0; i < LINKED_LIST_TEST_MAX_ITEMS_COUNT; i++)
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

int LinkedListTest::GetLinkedListItemCount()
{
    int count = 0;
    LinkedListItem * item = _linkedList._head;

    while (item != NULL)
    {
        count++;
        item = item->_next;
    }

    return count;
}

PUBLIC LinkedListTestItem newLinkedListTestItem(int number)
{
    LinkedListTestItem testItem = {
        .base = newLinkedListItem(),
        ._number = number
    };

    // overwrite equals method
    ListItem * listItem = (ListItem *)&testItem;
    listItem->equals = (ListItemEqualsMethod)equalsLinkedListTestItem;

    return testItem;
}

PUBLIC void deleteLinkedListTestItem(LinkedListTestItem * pThis)
{
    (void)pThis;
}

PUBLIC VIRTUAL bool equalsLinkedListTestItem(
        LinkedListTestItem * pThis,
        LinkedListTestItem * item)
{
    if (!IS_LINKED_LIST_TEST_ITEM((struct ListItem *)pThis))
    {
        return false;
    }

    if (!IS_LINKED_LIST_TEST_ITEM((struct ListItem *)item))
    {
        return false;
    }

    return (pThis->_number == item->_number);
}

PUBLIC STATIC bool findLinkedListTestItemCallback(LinkedListTestItem * item)
{
    if (!IS_LINKED_LIST_TEST_ITEM((struct ListItem *)item))
    {
        return false;
    }

    return (item->_number == LinkedListTest::findIndex);
}
