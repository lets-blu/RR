#ifndef __DISPLAYAREA_H__
#define __DISPLAYAREA_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stddef.h"
#include "stdint.h"

#include "cmsis_os.h"

#include "keywords.h"
#include "lcd1602.h"

typedef struct {
    LCD1602 * _lcd;
    LCDCursor _cursor;
    uint8_t _length;
    
    int8_t _scrollIndex;
    const char * _scrollString;
} DisplayArea;

// Constructor
PUBLIC DisplayArea newDisplayArea(LCD1602 * lcd);

// Public method(s)
PUBLIC void setDisplayAreaCursor(DisplayArea * pThis, LCDCursor cursor);
PUBLIC void setDisplayAreaLength(DisplayArea * pThis, uint8_t length);

PUBLIC void setDisplayAreaString(DisplayArea * pThis, const char * str);
PUBLIC void scrollDisplayArea(DisplayArea * pThis);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DISPLAYAREA_H__
