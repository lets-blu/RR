#ifndef __DISPLAYAREA_H__
#define __DISPLAYAREA_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "stddef.h"
#include "stdint.h"
#include "string.h"

#include "lcd1602.h"
#include "keywords.h"
#include "chainedobserver.h"

typedef struct {
    ChainedObserver base;

    uint8_t _length;
    LCDCursor _cursor;

    uint8_t _offset;
    const char * _string;
} DisplayArea;

// (De)constructor(s)
PUBLIC DisplayArea newDisplayArea(LCDCursor cursor, uint8_t length);

// Public method(s)
PUBLIC void setDisplayCursor(DisplayArea * pThis, LCDCursor cursor);
PUBLIC void setDisplayAreaString(DisplayArea * pThis, const char * str);

PUBLIC VIRTUAL void updateDisplayArea(DisplayArea * pThis, struct ISubject * subject);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DISPLAYAREA_H__
