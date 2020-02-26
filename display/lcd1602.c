#include "lcd1602.h"

#define LCD1602_BACKLIGHT           0x08
#define LCD1602_ENTRY_LEFT          0x02
#define LCD1602_DISPLAY_ON          0x04
#define LCD1602_FUNCTION_2LINES     0x08

#define LCD1602_DELAY(millisec)                 \
    osDelay(millisec)

#define LCD1602_CURRENT_CURSOR(lcd)             \
    (&((lcd)->_buffer[(lcd)->_cursor.row][(lcd)->_cursor.column]))

#define CLEAR_LCD1602(lcd)                      \
    write1ByteToLCD1602((lcd), 0x01, false);    \
    LCD1602_DELAY(1)

#define RETURN_LCD1602_HOME(lcd)                \
    write1ByteToLCD1602((lcd), 0x02, false);    \
    LCD1602_DELAY(1)

#define SET_LCD1602_ENTRY(lcd, entry)           \
    write1ByteToLCD1602((lcd), (0x04|(entry)), false)

#define SET_LCD1602_DISPLAY(lcd, display)       \
    write1ByteToLCD1602((lcd), (0x08|(display)), false)

#define SET_LCD1602_FUNCTION(lcd, function)     \
    write1ByteToLCD1602((lcd), (0x20|(function)), false)

#define SET_LCD1602_CURSOR(lcd, row, column)    \
    write1ByteToLCD1602((lcd), (0x80|((row)*0x40+(column))), false)

#define PRINT_LCD1602_LINE(lcd, row)            \
    for (uint8_t __i = 0; __i < 16; __i++)      \
        write1ByteToLCD1602((lcd), (lcd)->_buffer[(row)][__i], true)

// Private method(s)
PRIVATE void enableLCD16024BitMode(LCD1602 * pThis);

PRIVATE void write4BitsToLCD1602(LCD1602 * pThis, uint8_t data);
PRIVATE void write1ByteToLCD1602(LCD1602 * pThis, uint8_t data, bool isChar);

PRIVATE void writeLCD1602I2CData(LCD1602 * pThis, uint8_t data);
PRIVATE void pluseLCD1602Enable(LCD1602 * pThis, uint8_t data);

PRIVATE STATIC I2C_HandleTypeDef initializeLCD1602I2C(I2C_TypeDef * I2Cx);

PUBLIC LCD1602 newLCD1602(I2C_TypeDef * I2Cx, uint16_t address)
{
    assert(IS_I2C_ALL_INSTANCE(I2Cx));
    
    LCD1602 lcd = {
        ._cursor    = {0},
        ._buffer    = {{0}},
        
        ._address   = address,
        ._handle    = initializeLCD1602I2C(I2Cx)
    };

    clearLCD1602(&lcd);
    return lcd;
}

PUBLIC void initializeLCD1602(LCD1602 * pThis)
{
    osDelay(50);
    enableLCD16024BitMode(pThis);
    
    SET_LCD1602_FUNCTION(pThis, LCD1602_FUNCTION_2LINES);
    SET_LCD1602_DISPLAY(pThis, LCD1602_DISPLAY_ON);
    
    CLEAR_LCD1602(pThis);
    SET_LCD1602_ENTRY(pThis, LCD1602_ENTRY_LEFT);

    RETURN_LCD1602_HOME(pThis);
}

PUBLIC void clearLCD1602(LCD1602 * pThis)
{
    memset(pThis->_buffer, ' ', sizeof(pThis->_buffer));
}

PUBLIC void setLCD1602Cursor(LCD1602 * pThis, LCDCursor cursor)
{
    if (cursor.row < 2 && cursor.column < 16)
    {
        pThis->_cursor = cursor;
    }
}

PUBLIC void printLCD1602(LCD1602 * pThis, const char * str, uint8_t length)
{
    if (str != NULL && length > 0 && pThis->_cursor.column + length <= 16)
    {
        memcpy(LCD1602_CURRENT_CURSOR(pThis), str, length);
    }
}

PUBLIC void clearLCD1602WithLength(LCD1602 * pThis, uint8_t length)
{
    if (length > 0 && pThis->_cursor.column + length <= 16)
    {
        memset(LCD1602_CURRENT_CURSOR(pThis), ' ', length);
    }
}

PUBLIC void refreshLCD1602(LCD1602 * pThis)
{
    for (uint8_t i = 0; i < 2; i++)
    {
        SET_LCD1602_CURSOR(pThis, i, 0);
        PRINT_LCD1602_LINE(pThis, i);
    }
}

PRIVATE void enableLCD16024BitMode(LCD1602 * pThis)
{
    for (uint8_t i = 0; i < 3; i++)
    {
        write4BitsToLCD1602(pThis, 0x03 << 4);
        LCD1602_DELAY(1);
    }
    
    write4BitsToLCD1602(pThis, 0x02 << 4);
}

PRIVATE void write4BitsToLCD1602(LCD1602 * pThis, uint8_t data)
{
    writeLCD1602I2CData(pThis, data);
    pluseLCD1602Enable(pThis, data);
}

PRIVATE void write1ByteToLCD1602(LCD1602 * pThis, uint8_t data, bool isChar)
{
    write4BitsToLCD1602(pThis, (data & 0xf0) | isChar);
    write4BitsToLCD1602(pThis, ((data << 4) & 0xf0) | isChar);
}

PRIVATE void writeLCD1602I2CData(LCD1602 * pThis, uint8_t data)
{
    data |= LCD1602_BACKLIGHT;
    HAL_I2C_Master_Transmit(&pThis->_handle, pThis->_address, &data, 1, 0xffff);
}

PRIVATE void pluseLCD1602Enable(LCD1602 * pThis, uint8_t data)
{
    writeLCD1602I2CData(pThis, data | 0x04);
    LCD1602_DELAY(1);
    
    writeLCD1602I2CData(pThis, data & ~0x04);
    LCD1602_DELAY(1);
}

PRIVATE STATIC I2C_HandleTypeDef initializeLCD1602I2C(I2C_TypeDef * I2Cx)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    I2C_HandleTypeDef handle = {
        .Instance   = I2Cx,
        .Init       = {
            .ClockSpeed         = 100000,
            .DutyCycle          = I2C_DUTYCYCLE_2,
            .OwnAddress1        = 0,
            .AddressingMode     = I2C_ADDRESSINGMODE_7BIT,
            .DualAddressMode    = I2C_DUALADDRESS_DISABLE,
            .OwnAddress2        = 0,
            .GeneralCallMode    = I2C_GENERALCALL_DISABLE,
            .NoStretchMode      = I2C_NOSTRETCH_DISABLE
        }
    };
    
    HAL_I2C_Init(&handle);
    return handle;
}
