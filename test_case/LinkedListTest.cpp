#include "LinkedListTest.h"

// Static members
int LinkedListTest::findIndex = LINKED_LIST_TEST_FIND_ITEM_INDEX;

// Private methods
PRIVATE STATIC bool findLinkedNumberItemCallback(LinkedListItem * item);

TEST_F(LinkedListTest, add)
{
    // generate list
    GenerateLinkedList();
}

TEST_F(LinkedListTest, removeHead)
{
    // generate list
    GenerateLinkedList();

    // remove the head
    LinkedListItem * removeItem = removeLinkedListItem(
        &_linkedList,
        &_numberItems[0].base);

    // check result
    EXPECT_EQ(&_numberItems[0].base, removeItem);
    EXPECT_EQ(&_numberItems[1].base, _linkedList._head);
    EXPECT_EQ(LINKED_LIST_TEST_MAX_ITEM_COUNT - 1, GetLinkedListItemCount());

    // check list
    for (int i = 1; i < LINKED_LIST_TEST_MAX_ITEM_COUNT; i++)
    {
        if (i == LINKED_LIST_TEST_MAX_ITEM_COUNT - 1)
        {
            EXPECT_EQ(NULL, _numberItems->base._next);
        }
        else
        {
            EXPECT_EQ(&_numberItems[i + 1].base, _numberItems[i].base._next);
        }
    }
}

TEST_F(LinkedListTest, removeNonhead)
{
    // generate list
    GenerateLinkedList();

    // remove the non-head
    LinkedListItem * removeItem = removeLinkedListItem(
        &_linkedList,
        &_numberItems[LINKED_LIST_TEST_REMOVE_ITEM_INDEX].base);

    // check result
    EXPECT_EQ(&_numberItems[LINKED_LIST_TEST_REMOVE_ITEM_INDEX].base, removeItem);
    EXPECT_EQ(&_numberItems[0].base, _linkedList._head);
    EXPECT_EQ(LINKED_LIST_TEST_MAX_ITEM_COUNT - 1, GetLinkedListItemCount());

    // check list
    for (int i = 0; i < LINKED_LIST_TEST_MAX_ITEM_COUNT; i++)
    {
        if (i == LINKED_LIST_TEST_REMOVE_ITEM_INDEX)
        {
            continue;
        }
        else if (i == LINKED_LIST_TEST_REMOVE_ITEM_INDEX - 1)
        {
            if (i == LINKED_LIST_TEST_MAX_ITEM_COUNT - 2)
            {
                EXPECT_EQ(NULL, _numberItems[i].base._next);
            }
            else
            {
                EXPECT_EQ(&_numberItems[i + 2].base, _numberItems[i].base._next);
            }
        }
        else
        {
            if (i == LINKED_LIST_TEST_MAX_ITEM_COUNT - 1)
            {
                EXPECT_EQ(NULL, _numberItems[i].base._next);
            }
            else
            {
                EXPECT_EQ(&_numberItems[i + 1].base, _numberItems[i].base._next);
            }
        }
    }
}

TEST_F(LinkedListTest, find)
{
    // generate list
    GenerateLinkedList();

    // find the item
    LinkedListItem * findItem = findLinkedListItem(
        &_linkedList,
        findLinkedNumberItemCallback);

    // check result
    EXPECT_EQ(&_numberItems[LINKED_LIST_TEST_FIND_ITEM_INDEX].base, findItem);
}

void LinkedListTest::SetUp()
{
    _linkedList = newLinkedList();

    for (int i = 0; i < LINKED_LIST_TEST_MAX_ITEM_COUNT; i++)
    {
        _numberItems[i] = newLinkedNumberItem(i);
    }
}

void LinkedListTest::TearDown()
{
    deleteLinkedList(&_linkedList);

    for (int i = 0; i < LINKED_LIST_TEST_MAX_ITEM_COUNT; i++)
    {
        deleteLinkedNumberItem(&_numberItems[i]);
    }
}

void LinkedListTest::GenerateLinkedList()
{
    // add items
    for (int i = LINKED_LIST_TEST_MAX_ITEM_COUNT; i > 0; i--)
    {
        EXPECT_EQ(0, addLinkedListItem(&_linkedList, &_numberItems[i - 1].base));
    }

    // check result
    EXPECT_EQ(&_numberItems[0].base, _linkedList._head);
    EXPECT_EQ(LINKED_LIST_TEST_MAX_ITEM_COUNT, GetLinkedListItemCount());

    // check list
    for (int i = 0; i < LINKED_LIST_TEST_MAX_ITEM_COUNT; i++)
    {
        if (i == LINKED_LIST_TEST_MAX_ITEM_COUNT - 1)
        {
            EXPECT_EQ(NULL, _numberItems[i].base._next);
        }
        else
        {
            EXPECT_EQ(&_numberItems[i + 1].base, _numberItems[i].base._next);
        }
    }
}

int LinkedListTest::GetLinkedListItemCount()
{
    int count = 0;

    for (LinkedListItem * item = _linkedList._head; item != NULL; item = item->_next)
    {
        count++;
    }

    return count;
}

PUBLIC LinkedNumberItem newLinkedNumberItem(int number)
{
    LinkedNumberItem item = {
        .base		= newLinkedListItem(),
        ._number	= number
    };

    struct IListItem * listItem = (struct IListItem *)&item;
    listItem->equals = (IListItemEqualsMethod)equalsLinkedNumberItem;

    return item;
}

PUBLIC void deleteLinkedNumberItem(LinkedNumberItem * pThis)
{
    memset(pThis, 0, sizeof(LinkedNumberItem));
}

PUBLIC bool equalsLinkedNumberItem(LinkedNumberItem * pThis, LinkedNumberItem * item)
{
    if (!IS_LINKED_NUMBER_ITEM(pThis) || !IS_LINKED_NUMBER_ITEM(item))
    {
        return false;
    }

    return pThis->_number == item->_number;
}

PRIVATE STATIC bool findLinkedNumberItemCallback(LinkedListItem * item)
{
    if (!IS_LINKED_NUMBER_ITEM(item))
    {
        return false;
    }

    return ((LinkedNumberItem *)item)->_number == LinkedListTest::findIndex;
}
