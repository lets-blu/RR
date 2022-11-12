#include "syslog/inc/LogPrinterManager.h"

#define IS_LOG_PRINTER_MANAGER_CONSTRUCTED(manager) \
    IS_ARRAY_LIST_CONSTRUCTED(&(manager)->_printersList)

// Private member(s)
PRIVATE STATIC LogPrinterManager instance;
PRIVATE STATIC const char *findPrinterName;

// Private method(s)
PRIVATE void constructLogPrinterManager(LogPrinterManager *instance);

PRIVATE void constructLogPrinterManagerItem(
    LogPrinterManagerItem *instance,
    const char *name,
    struct ILogPrinter *printer);

PRIVATE void deconstructLogPrinterManagerItem(LogPrinterManagerItem *instance);
PRIVATE STATIC bool findPrinterCallbackOfLogPrinterManager(ArrayListItem *item);

PRIVATE void constructLogPrinterManager(LogPrinterManager *instance)
{
    instance->_defaultPrinter = NULL;

    constructArrayList(
        &instance->_printersList,
        MAX_LOG_PRINTER_MANAGER_PRINTERS_NUMBER,
        sizeof(LogPrinterManagerItem));
}

PRIVATE void constructLogPrinterManagerItem(
    LogPrinterManagerItem *instance,
    const char *name,
    struct ILogPrinter *printer)
{
    instance->_name = name;
    instance->_printer = printer;
    constructArrayListItem(&instance->arrayListItem);

    struct IListItem *listItem = (struct IListItem *)&instance;
    listItem->equals = (IListItemEqualsMethod)equalsBetweenLogPrinterManagerItem;
}

PRIVATE void deconstructLogPrinterManagerItem(LogPrinterManagerItem *instance)
{
    deconstructArrayListItem(&instance->arrayListItem);
    memset(instance, 0, sizeof(LogPrinterManagerItem));
}

PUBLIC bool addPrinterToLogPrinterManager(
    LogPrinterManager *pThis,
    const char *name,
    struct ILogPrinter *printer)
{
    int index = 0;
    LogPrinterManagerItem newItem = {0};
    LogPrinterManagerItem *existItem = NULL;

    // 1. Check parameters
    if (pThis == NULL || name == NULL || printer == NULL)
    {
        return false;
    }

    // 2. Check printer exist
    findPrinterName = name;

    existItem = (LogPrinterManagerItem *)findItemFromArrayList(
        &pThis->_printersList,
        findPrinterCallbackOfLogPrinterManager);

    // 3. Add printer to list
    if (existItem != NULL)
    {
        return false;
    }

    constructLogPrinterManagerItem(&newItem, name, printer);
    index = addItemToArrayList(&pThis->_printersList, &newItem.arrayListItem);
    deconstructLogPrinterManagerItem(&newItem);

    return (index != -1);
}

PUBLIC void removePrinterFromLogPrinterManager(
    LogPrinterManager *pThis,
    const char *name)
{
    LogPrinterManagerItem *item = NULL;

    // 1. Check parameters
    if (pThis == NULL || name == NULL)
    {
        return;
    }

    // 2. Find item
    findPrinterName = name;

    item = (LogPrinterManagerItem *)findItemFromArrayList(
        &pThis->_printersList,
        findPrinterCallbackOfLogPrinterManager);

    // 3. Remove item
    if (item == NULL)
    {
        return;
    }

    if (pThis->_defaultPrinter == item->_printer)
    {
        pThis->_defaultPrinter = NULL;
    }

    removeItemFromArrayList(&pThis->_printersList, &item->arrayListItem);
}

PUBLIC void setDefaultPrinterToLogPrinterManager(
    LogPrinterManager *pThis,
    const char *name)
{
    LogPrinterManagerItem *item = NULL;

    // 1. Check parameter
    if (pThis == NULL)
    {
        return;
    }

    // 2. Reset default printer
    if (name == NULL)
    {
        pThis->_defaultPrinter = NULL;
    }

    // 3. Find item
    findPrinterName = name;

    item = (LogPrinterManagerItem *)findItemFromArrayList(
        &pThis->_printersList,
        findPrinterCallbackOfLogPrinterManager);

    if (item != NULL)
    {
        pThis->_defaultPrinter = item->_printer;
    }
}

PUBLIC struct ILogPrinter *getDefaultPrinterFromLogPrinterManager(
    LogPrinterManager *pThis)
{
    if (pThis == NULL)
    {
        return NULL;
    }

    return pThis->_defaultPrinter;
}

PUBLIC bool equalsBetweenLogPrinterManagerItem(
    LogPrinterManagerItem *pThis,
    LogPrinterManagerItem *item)
{
    if (pThis == NULL || item == NULL)
    {
        return false;
    }

    if (pThis->_name == NULL || item->_name == NULL)
    {
        return false;
    }

    return (strcmp(pThis->_name, item->_name) == 0);
}

PUBLIC STATIC LogPrinterManager *instanceOfLogPrinterManager(void)
{
    if (!IS_LOG_PRINTER_MANAGER_CONSTRUCTED(&instance))
    {
        constructLogPrinterManager(&instance);
    }

    return &instance;
}

PRIVATE STATIC bool findPrinterCallbackOfLogPrinterManager(ArrayListItem *item)
{
    LogPrinterManagerItem *printerItem = (LogPrinterManagerItem *)item;

    if (printerItem == NULL)
    {
        return false;
    }

    return (strcmp(printerItem->_name, findPrinterName) == 0);
}
