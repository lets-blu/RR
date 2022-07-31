#include "ArrayListTest.h"

// Private methods 
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
        &_testItems[ARRAY_LIST_TEST_REMOVE_ITEM_INDEX].base);

    // check result
    EXPECT_EQ(&_testItems[ARRAY_LIST_TEST_REMOVE_ITEM_INDEX].base, removeItem);
    EXPECT_EQ((size_t)ARRAY_LIST_TEST_MAX_ITEMS_COUNT - 1, _arrayList._itemsCount);

    // check list
    CheckArrayList(ARRAY_LIST_TEST_REMOVE_ITEM_INDEX);
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
    ArrayListTestItem * item = IListItem2ArrayListTestItem(findItem);
    EXPECT_EQ(ARRAY_LIST_TEST_FIND_ITEM_INDEX, item->_number);
}

TEST_F(ArrayListTest, removeAt)
{
    // generate list
    GenerateArrayList();

    // remove the item
    ArrayListTestItem item = {{0}, 0};

    bool isRemove = removeArrayListItemAt(
        &_arrayList,
        ARRAY_LIST_TEST_REMOVE_ITEM_INDEX,
        &item.base);

    // check result
    EXPECT_TRUE(isRemove);
    EXPECT_EQ(ARRAY_LIST_TEST_REMOVE_ITEM_INDEX, item._number);
    EXPECT_EQ((size_t)ARRAY_LIST_TEST_MAX_ITEMS_COUNT - 1, _arrayList._itemsCount);

    // check list
    CheckArrayList(ARRAY_LIST_TEST_REMOVE_ITEM_INDEX);
}

TEST_F(ArrayListTest, getItemsCount)
{
    // test in other cases
}

void ArrayListTest::SetUp()
{
    _arrayList = newArrayList(ARRAY_LIST_TEST_MAX_ITEMS_COUNT, sizeof(ArrayListTestItem));

    for (int i = 0; i < ARRAY_LIST_TEST_MAX_ITEMS_COUNT; i++)
    {
        _testItems[i] = newArrayListTestItem(i);
    }
}

void ArrayListTest::TearDown()
{
    deleteArrayList(&_arrayList);

    for (int i = 0; i < ARRAY_LIST_TEST_MAX_ITEMS_COUNT; i++)
    {
        deleteArrayListTestItem(&_testItems[i]);
    }
}

void ArrayListTest::GenerateArrayList()
{
    // add items
    for (int i = 0; i < ARRAY_LIST_TEST_MAX_ITEMS_COUNT; i++)
    {
        EXPECT_EQ(
            getArrayListItemsCount(&_arrayList) - 1,
            (size_t)addArrayListItem(&_arrayList, &_testItems[i].base));
    }

    // check result
    EXPECT_EQ((size_t)ARRAY_LIST_TEST_MAX_ITEMS_COUNT, _arrayList._itemsCount);

    // check list
    CheckArrayList(ARRAY_LIST_TEST_MAX_ITEMS_COUNT);
}

void ArrayListTest::CheckArrayList(int removeIndex)
{
    int number = 0;
    uint8_t * cursor = _arrayList._base;

    for (int i = 0; (size_t)i < _arrayList._itemsCount; i++)
    {
        ArrayListTestItem * item = (ArrayListTestItem *)cursor;

        if (i == removeIndex)
        {
            number++;
        }

        EXPECT_EQ(number, item->_number);
        number++;
        cursor += _arrayList._itemSize;
    }
}

PUBLIC ArrayListTestItem newArrayListTestItem(int number)
{
    ArrayListTestItem item = {
        .base       = newArrayListItem(),
        ._number    = number
    };

    struct IListItem * listItem = ArrayListTestItem2IListItem(&item);
    listItem->equals = (IListItemEqualsMethod)equalsArrayListTestItem;

    return item;
}

PUBLIC void deleteArrayListTestItem(ArrayListTestItem * pThis)
{
    deleteArrayListItem(&pThis->base);
    memset(pThis, 0, sizeof(ArrayListTestItem));
}

PUBLIC bool equalsArrayListTestItem(
    ArrayListTestItem * pThis,
    ArrayListTestItem * item)
{
    if (!isArrayListTestItem(ArrayListTestItem2IListItem(item)))
    {
        return false;
    }

    return (pThis->_number == item->_number);
}

PRIVATE STATIC bool findArrayNumberItemCallback(ArrayListItem * item)
{
    ArrayListTestItem * testItem  = (ArrayListTestItem *)item;

    if (!isArrayListTestItem(ArrayListTestItem2IListItem(item)))
    {
        return false;
    }

    return (testItem->_number == ARRAY_LIST_TEST_FIND_ITEM_INDEX);
}
