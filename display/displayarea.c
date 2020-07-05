#include "displayarea.h"

#define DISPLAY_AREA_MIN(a, b) ((a) < (b) ? (a) : (b))

// Private method(s)
PRIVATE void refreshDisplayAreaLCD(DisplayArea * pThis, const char * str, uint8_t length);

PUBLIC DisplayArea newDisplayArea(LCD1602 * lcd)
{
    DisplayArea area = {
        ._lcd               = lcd,
        ._cursor            = {0},
        ._length            = 0,
        
        ._scrollIndex       = 0,
        ._scrollString      = NULL
    };

    return area;
}

PUBLIC void setDisplayAreaCursor(DisplayArea * pThis, LCDCursor cursor)
{
    pThis->_cursor = cursor;
}

PUBLIC void setDisplayAreaLength(DisplayArea * pThis, uint8_t length)
{
    pThis->_length = length;
}

PUBLIC void setDisplayAreaString(DisplayArea * pThis, const char * str)
{
    pThis->_scrollIndex = -1;
    pThis->_scrollString = str;
    refreshDisplayAreaLCD(pThis, str, DISPLAY_AREA_MIN(strlen(str), pThis->_length));
}

PUBLIC void scrollDisplayArea(DisplayArea * pThis)
{
    if (pThis->_scrollString == NULL)
    {
        return;
    }

    if (pThis->_scrollIndex < strlen(pThis->_scrollString))
    {
        pThis->_scrollIndex++;
    }
    else
    {
        pThis->_scrollIndex = 0;
    }

    const char * str = pThis->_scrollString + pThis->_scrollIndex;
    const uint8_t length = strlen(pThis->_scrollString) - pThis->_scrollIndex;
    refreshDisplayAreaLCD(pThis, str, DISPLAY_AREA_MIN(length, pThis->_length));
}

PRIVATE void refreshDisplayAreaLCD(DisplayArea * pThis, const char * str, uint8_t length)
{
    setLCD1602Cursor(pThis->_lcd, pThis->_cursor);
    clearLCD1602WithLength(pThis->_lcd, pThis->_length);

    printLCD1602(pThis->_lcd, str, length);
    refreshLCD1602(pThis->_lcd);
}
