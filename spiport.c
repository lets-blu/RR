#include "spiport.h"

#define __NUM_OF_PINS	((uint8_t)4)
#define __NSS_PIN		((uint8_t)0)
#define __SCK_PIN		((uint8_t)1)
#define __MISO_PIN		((uint8_t)2)
#define __MOSI_PIN		((uint8_t)3)

// private method(s)
PRIVATE void initSPIPortPins(SPIPort * this);
PRIVATE void setSPIPortClock(SPIPort * this, FunctionalState newState);

PUBLIC SPIPort newSPIPort(SPI_TypeDef * spi) {
    return spi;
}

PUBLIC void initSPIPort(SPIPort * this) {
	initSPIPortPins(this);
	setSPIPortClock(this, ENABLE);
	
	SPI_InitTypeDef SPI_InitStructure = {
		.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2, 
		.SPI_Direction= SPI_Direction_2Lines_FullDuplex, 
		.SPI_Mode = SPI_Mode_Master, 
		.SPI_DataSize = SPI_DataSize_8b, 
		.SPI_CPOL = SPI_CPOL_Low, 
		.SPI_CPHA = SPI_CPHA_1Edge, 
		.SPI_NSS = SPI_NSS_Soft, 
		.SPI_FirstBit = SPI_FirstBit_MSB, 
		.SPI_CRCPolynomial = 7
	};
	
	SPI_Init(*this, &SPI_InitStructure);
	SPI_SSOutputCmd(*this, ENABLE);
	SPI_Cmd(*this, ENABLE);
}

PRIVATE void initSPIPortPins(SPIPort * this) {
	GPIOPin pins[__NUM_OF_PINS];
	
 	if (*this == SPI1) {
 		pins[__NSS_PIN] = newGPIOPin(GPIOA, GPIO_Pin_4);
 		pins[__SCK_PIN] = newGPIOPin(GPIOA, GPIO_Pin_5);
 		pins[__MISO_PIN] = newGPIOPin(GPIOA, GPIO_Pin_6);
 		pins[__MOSI_PIN] = newGPIOPin(GPIOA, GPIO_Pin_7);
 	} else {
 		if (*this == SPI2) {
 			pins[__NSS_PIN] = newGPIOPin(GPIOB, GPIO_Pin_12);
 			pins[__SCK_PIN] = newGPIOPin(GPIOB, GPIO_Pin_13);
 			pins[__MISO_PIN] = newGPIOPin(GPIOB, GPIO_Pin_14);
 			pins[__MOSI_PIN] = newGPIOPin(GPIOB, GPIO_Pin_15);
 		}
 	}
	
 	for (uint8_t i = 0; i < __NUM_OF_PINS; i++) {
 		switch (i) {
 			case __NSS_PIN:
 				configGPIOPin(pins + i, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
 				writeGPIOPin(pins + i, HIGH);
 				break;

 			case __SCK_PIN:
 			case __MISO_PIN:
 			case __MOSI_PIN:
 				configGPIOPin(pins + i, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);
 				break;
 		}
 	}
}

PRIVATE void setSPIPortClock(SPIPort * this, FunctionalState newState) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
 	if (*this == SPI1) {
 		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, newState);
 	} else {
 		if (*this == SPI2) {
 			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, newState);
 		}
 	}
}
