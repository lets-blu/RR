/// <summary>
/// 	Header(s)
/// </summary>
#include "stdbool.h"
#include "stm32f10x.h"
#include "stm32f10x_exti.h"

// required for testing button and led
#include "button.h"
#include "led.h"

// required for testing shift register
#include "registergroup.h"
#include "shiftregister.h"

// required for testing w5500 ethernet
#include "ethernet.h"
#include "w5500.h"
#include "w5500_porting.h"

// required for testing watchdog timer
#include "watchdog.h"


/// <summary>
/// 	Variable(s)
/// </summary>
bool interrupt;
Button button;
LED led;

RegisterGroup registerGroup;
ShiftRegister shiftRegister;

W5500 w5500;
uint8_t rxBuffer[255];
uint8_t txBuffer[255];

Watchdog watchdog;


/// <summary>
/// 	Function declaration(s)
/// </summary>
void initButtonLED(void);
void testButtonLED(void);
void onButtonClick(Button * this, void * arg);

void initShiftRegister(void);
void testShiftRegister(void);

void initEthernet(void);
void testEthernet(void);
void onW5500Receive(W5500 * this);

void initWatchdog(void);
void testWatchdog(void);

void delay(uint32_t delay);


/// <summary>
/// 	Function implement(s)
/// </summary>
int main(void)
{
	// Cancel the comment which you want to test.
//	initButtonLED();
//	initShiftRegister();
//	initEthernet();
	initWatchdog();
	
	for (;;)
	{
//		testButtonLED();
//		testShiftRegister();
//		testEthernet();
		testWatchdog();
	}
}

/// <summary>
/// 	Initialize button and led, 
/// 	Must be called before calling testButtonLED().
/// </summary>
void initButtonLED(void)
{
	interrupt = false;
	
	led = newLED(newGPIOPin(GPIOC, GPIO_Pin_13), LOW);
	button = newButton(newGPIOPin(GPIOB, GPIO_Pin_0), LOW);
	
	button.onClick = onButtonClick;
	setButtonInterrupt(&button, 0, 0, ENABLE);
}

/// <summary>
///		Test button and led, 
/// 	Press button on PB0, led on PC13 while be lighted up for a while.
/// 	Must call initButtonLED() before calling.
/// </summary>
void testButtonLED(void)
{
	if (interrupt && isButtonClicked(&button))
	{
		button.onClick(&button, &led);
		delay(0x3fffff);
	}
	
	blackOutLED(&led);
}

/// <summary>
/// 	Handle button clicked event, 
/// 	If it doesn't implement, default handler will do nothing when button clicked.
/// </summary>
void onButtonClick(Button * this, void * arg)
{
	lightUpLED((LED *)arg);
}

/// <summary>
/// 	Initialize shift register, 
/// 	Must be called before calling testShiftRegister().
/// </summary>
void initShiftRegister(void)
{
	shiftRegister = newShiftRegister();
	registerGroup = newRegisterGroup((GPIOPin []){
		[REGGRP_SER_PIN] = newGPIOPin(GPIOA, GPIO_Pin_7), 
		[REGGRP_SCK_PIN] = newGPIOPin(GPIOA, GPIO_Pin_5), 
		[REGGRP_RCK_PIN] = newGPIOPin(GPIOA, GPIO_Pin_6), 
		[REGGRP_E_PIN]   = newGPIOPin(GPIOA, GPIO_Pin_4)
	});
	
	// shift register must be belonged to a register group
	addRegisterToGroup(&registerGroup, &shiftRegister);
	setRegisterGroupEnable(&registerGroup, ENABLE);
}

/// <summary>
/// 	Test shift register,
/// 	It will show a light water in first shift register.
/// 	Must call initShiftRegister() before calling.
/// </summary>
void testShiftRegister(void)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		// clear all bits
		for (uint8_t j = 0; j < 8; j++)
		{
			resetRegisterGroupBit(&registerGroup, j);
		}
		
		setRegisterGroupBit(&registerGroup, i);
		outputRegisterGroup(&registerGroup);
		delay(0x3fffff);
	}
}

/// <summary>
/// 	Initialize ethernet with W5500 chip, 
/// 	Must called before testEthernet().
/// </summary>
void initEthernet(void)
{
	w5500 = newW5500(SPI2);
	
	setEthernetClientParams(&w5500.base, 
		(uint8_t []){192, 168, 1, 199}, 50000);
	setEthernetServerParams(&w5500.base, 
		(uint8_t []){192, 168, 1, 107}, 60000);
	setEthernetNetworkParams(&w5500.base, 
		(uint8_t []){255, 255, 255, 0}, (uint8_t []){192, 168, 1, 1});
	setEthernetRxBuffer(&w5500.base, rxBuffer, 255);
	setEthernetTxBuffer(&w5500.base, txBuffer, 255);
	
	w5500.cris_en = SPI_CrisEnter;
	w5500.cris_ex = SPI_CrisExit;
	w5500.cs_sel = SPI_CS_Select;
	w5500.cs_desel = SPI_CS_Deselect;
	w5500.spi_rb = SPI_ReadByte;
	w5500.spi_wb = SPI_WriteByte;
	
	w5500.base.onReceive = (void (*)(struct Ethernet *))onW5500Receive;
	w5500.base.init(&w5500.base);
}

/// <summary>
/// 	Test ethernet with W5500 Chip, 
/// 	It will echo the message which from server to server.
/// 	Must call initEthernet() before calling.
/// </summary>
void testEthernet(void)
{
	syncW5500(&w5500);
}

/// <summary>
/// 	Handle ethernet with W5500 chip event, 
/// 	If it doesn't implement, default handler will do nothing when ethernet received.
/// </summary>
void onW5500Receive(W5500 * this)
{
	this->base.send(&w5500.base, w5500.base._rxBuffer, w5500.base._numOfRxData);
	this->base._numOfRxData = 0;
}

/// <summary>
/// 	Initialize watchdog timer, 
/// 	Must called before testWatchdog().
/// </summary>
void initWatchdog(void)
{
	watchdog = newWatchdog(1000);
	watchdog.init(&watchdog);
	
	// used to indicate reboot
	led = newLED(newGPIOPin(GPIOC, GPIO_Pin_13), LOW);
	
	lightUpLED(&led);
	delay(0x3fffff);
	blackOutLED(&led);
}

/// <summary>
/// 	Test watchdog timer, 
/// 	It will do nothing, waiting for reboot.
/// 	Must call initWatchdog() before calling.
/// </summary>
void testWatchdog(void)
{
	
}

/// <summary>
/// 	A simple delay
/// </summary>
void delay(uint32_t delay)
{
	for (; delay; --delay);
}

/// <summary>
/// 	EXTI0 interrupt handler, 
/// 	It's used to handle button on PB0 press interrupt
/// </summary>
void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0))
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
		interrupt = true;
	}
}
