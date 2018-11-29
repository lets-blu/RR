/**
  ******************************************************************************
  * @file    spi.c
  * $Author: ???? $
  * $Revision: 17 $
  * $Date:: 2014-10-25 11:16:48 +0800 #$
  * @brief   SPI??????.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, EmbedNet</center>
  *<center><a href="http:\\www.embed-net.com">http://www.embed-net.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
//#include "main.h"
#include "stm32f10x_spi.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  ??SPI??
  * @retval None
  */
static void SPI_RCC_Configuration(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
}

/**
  * @brief  ????SPI???
  * @retval None
  */
static void SPI_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  			 
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStruct);
  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

/**
  * @brief  ????SPI????SPI????
  * @retval None
  */
void SPI_Configuration(void)
{
	SPI_InitTypeDef SPI_InitStruct;

	SPI_RCC_Configuration();
	SPI_GPIO_Configuration();
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2,&SPI_InitStruct);
	
	SPI_SSOutputCmd(SPI2, ENABLE);
	SPI_Cmd(SPI2, ENABLE);
}

/**
  * @brief  ?1?????SPI??
  * @param  TxData ???????
  * @retval None
  */
void SPI_WriteByte(uint8_t TxData)
{				 
	while((SPI2->SR&SPI_I2S_FLAG_TXE)==0);	//??????		  
	SPI2->DR=TxData;	 	  									//????byte 

	while((SPI2->SR&SPI_I2S_FLAG_RXNE)==0); //???????byte  
	SPI2->DR;		
}

/**
  * @brief  ?SPI????1????
  * @retval ?????
  */
uint8_t SPI_ReadByte(void)
{			 
	while((SPI2->SR&SPI_I2S_FLAG_TXE)==0);	//??????			  
	SPI2->DR=0xFF;	 	  										//???????????????? 
	while((SPI2->SR&SPI_I2S_FLAG_RXNE)==0); //???????byte  
	return SPI2->DR;  
}

/**
  * @brief  ?????
  * @retval None
  */
void SPI_CrisEnter(void)
{
	__set_PRIMASK(1);
}

/**
  * @brief  ?????
  * @retval None
  */
void SPI_CrisExit(void)
{
	__set_PRIMASK(0);
}

/**
  * @brief  ?????????
  * @retval None
  */
void SPI_CS_Select(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}

/**
  * @brief  ?????????
  * @retval None
  */
void SPI_CS_Deselect(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}
/*********************************END OF FILE**********************************/
