#ifndef __LCD1602_H__
#define __LCD1602_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "assert.h"
#include "stdbool.h"
#include "stdint.h"
#include "string.h"

#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_i2c.h"
#include "stm32f1xx_hal_rcc.h"

#include "keywords.h"
#include "chainedobserver.h"

typedef struct {
    uint8_t row;
    uint8_t column;
} LCDCursor;

typedef struct {
    struct ISubject subject;
    ChainedObserver * _observers;

    uint16_t _address;
    I2C_HandleTypeDef _handle;
    osThreadId _refreshThread;

    LCDCursor _cursor;
    char _buffer[2][16];
} LCD1602;

// Constructor
PUBLIC LCD1602 newLCD1602(I2C_TypeDef * I2Cx, uint16_t address);

// Public method(s)
PUBLIC void initializeLCD1602(LCD1602 * pThis);

PUBLIC void clearLCD1602(LCD1602 * pThis);
PUBLIC void clearLCD1602WithLength(LCD1602 * pThis, uint8_t length);

PUBLIC void setLCD1602Cursor(LCD1602 * pThis, LCDCursor cursor);
PUBLIC void printLCD1602(LCD1602 * pThis, const char * str, uint8_t length);

PUBLIC void refreshLCD1602(LCD1602 * pThis);

PUBLIC bool isLCD1602RefreshEnabled(LCD1602 * pThis);
PUBLIC void setLCD1602RefreshEnabled(LCD1602 * pThis, bool enabled);

PUBLIC VIRTUAL void registerLCD1602Observer(LCD1602 * pThis, 
    struct IObserver * observer);
PUBLIC VIRTUAL void deregisterLCD1602Observer(LCD1602 * pThis, 
    struct IObserver * observer);
PUBLIC VIRTUAL void notifyLCD1602Observers(LCD1602 * pThis);

PUBLIC STATIC void vRefreshLCD1602Thread(void const * argument);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LCD1602_H__
