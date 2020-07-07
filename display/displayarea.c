#include "displayarea.h"

#define DISPLAY_AREA_MIN(a, b) (((a) < (b)) ? (a) : (b))

PUBLIC DisplayArea newDisplayArea(LCDCursor cursor, uint8_t length)
{
    DisplayArea area = {
        .base       = newChainedObserver(),

        ._cursor    = cursor,
        ._length    = length,

        ._offset    = 0,
        ._string    = NULL
    };

    area.base.observer.update = (update_observer_fp)updateDisplayArea;

    return area;
}

PUBLIC void setDisplayAreaString(DisplayArea * pThis, const char * str)
{
    pThis->_offset = 0;
    pThis->_string = str;
}

PUBLIC VIRTUAL void updateDisplayArea(DisplayArea * pThis, struct ISubject * subject)
{
    LCD1602 * lcd = (LCD1602 *)subject;

    if (pThis->_string != NULL)
    {
        const char * str = pThis->_string + pThis->_offset;
        uint8_t length = strlen(pThis->_string) - pThis->_offset;

        setLCD1602Cursor(lcd, pThis->_cursor);
        
        clearLCD1602WithLength(lcd, pThis->_length);
        printLCD1602(lcd, str, DISPLAY_AREA_MIN(pThis->_length, length));

        if (pThis->_length < strlen(pThis->_string))
        {
            pThis->_offset = (pThis->_offset + 1) % strlen(pThis->_string);
        }
    }
}
