#include "porting.h"

void SPI2_Cris_Enter(void)
{
    __set_PRIMASK(1);
}

void SPI2_Cris_Exit(void)
{
    __set_PRIMASK(0);
}

void SPI2_CS_Select(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}

void SPI2_CS_Deselect(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
}

uint8_t SPI2_ReadByte(void)
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI2, 0xff);

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI2);
}

void SPI2_WriteByte(uint8_t wb)
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI2, wb);

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(SPI2);
}
