#include "ArrayListTest.h"

// Private members
PRIVATE STATIC int findIndex = ARRAY_LIST_TEST_FIND_ITEM_INDEX;
PRIVATE STATIC bool findArrayNumberItemCallback(ArrayListItem * item);

TEST_F(ArrayListTest, add)
{
    // generate list
    GenerateArrayList();
}

TEST_F(ArrayListTest, remove)
{
    // generate list
    GenerateArrayList();

    // remove item
    ArrayListItem * removeItem = removeArrayListItem(
        &_arrayList,
        &_numberItems[ARRAY_LIST_TEST_REMOVE_ITEM_INDEX].base);

    // check result
    EXPECT_EQ(&_numberItems[ARRAY_LIST_TEST_REMOVE_ITEM_INDEX].base, removeItem);
    EXPECT_EQ(ARRAY_LIST_TEST_MAX_ITEM_COUNT - 1, GetArrayListItemCount());
}

TEST_F(ArrayListTest, find)
{
    // generate list
    GenerateArrayList();

    // find item
    ArrayListItem * findItem = findArrayListItem(
        &_arrayList,
        findArrayNumberItemCallback);

    // check result
    struct IListItem * listItem
        = (struct IListItem *)&_numberItems[ARRAY_LIST_TEST_FIND_ITEM_INDEX];

    EXPECT_TRUE(listItem->equals(listItem, &findItem->listItem));
}

void ArrayListTest::SetUp()
{
    _arrayList = newArrayList(ARRAY_LIST_TEST_MAX_ITEM_COUNT, sizeof(ArrayNumberItem));

    for (int i = 0; i < ARRAY_LIST_TEST_MAX_ITEM_COUNT; i++)
    {
        _numberItems[i] = newArrayNumberItem(i);
    }
}

void ArrayListTest::TearDown()
{
    deleteArrayList(&_arrayList);

    for (int i = 0; i < ARRAY_LIST_TEST_MAX_ITEM_COUNT; i++)
    {
        deleteArrayNumberItem(&_numberItems[i]);
    }
}

void ArrayListTest::GenerateArrayList()
{
    // add items
    for (int i = 0; i < ARRAY_LIST_TEST_MAX_ITEM_COUNT; i++)
    {
        EXPECT_EQ(
            GetArrayListItemCount() - 1,
            addArrayListItem(&_arrayList, &_numberItems[i].base));
    }

    // check result
    EXPECT_EQ(ARRAY_LIST_TEST_MAX_ITEM_COUNT, GetArrayListItemCount());
}

int ArrayListTest::GetArrayListItemCount()
{
    return _arrayList._itemsCount;
}

PUBLIC ArrayNumberItem newArrayNumberItem(int number)
{
    ArrayNumberItem item = {
        .base		= newArrayListItem(),
        ._number	= number
    };

    struct IListItem * listItem = (struct IListItem *)&item;
    listItem->equals = (IListItemEqualsMethod)equalsArrayNumberItem;

    return item;
}

PUBLIC void deleteArrayNumberItem(ArrayNumberItem * pThis)
{
    memset(pThis, 0, sizeof(ArrayNumberItem));
}

PUBLIC bool equalsArrayNumberItem(ArrayNumberItem * pThis, ArrayNumberItem * item)
{
    if (!IS_ARRAY_NUMBER_ITEM(pThis) || !IS_ARRAY_NUMBER_ITEM(item))
    {
        return false;
    }

    return pThis->_number == item->_number;
}

PRIVATE STATIC bool findArrayNumberItemCallback(ArrayListItem * item)
{
    if (!IS_ARRAY_NUMBER_ITEM(item))
    {
        return false;
    }

    return ((ArrayNumberItem *)item)->_number == findIndex;
}
