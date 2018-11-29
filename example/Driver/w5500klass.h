#ifndef __W5500CLASS_H__
#define __W5500CLASS_H__

#include "FreeRTOS.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stm32f10x_spi.h"
#include "string.h"
#include "task.h"

#include "ethernet.h"
#include "gpiopin.h"
#include "watchdog.h"

typedef struct W5500Class {
	struct Ethernet base; // super class
	
	void (*crisEnter)(void);
	void (*crisExit)(void);
	void (*csSelect)(void);
	void (*csDeselect)(void);
	uint8_t (*spiReadByte)(void);
	void (*spiWriteByte)(uint8_t writeByte);
} W5500Class;

PUBLIC W5500Class * newW5500Class(SPI_TypeDef * spi);

PUBLIC VIRTUAL bool initW5500(struct Ethernet * this);
PUBLIC VIRTUAL void onW5500Receive(struct Ethernet * this);
PUBLIC VIRTUAL uint8_t sendW5500Message(struct Ethernet * this, 
		uint8_t type, EthernetObject objects[], uint8_t numOfObjects);

PUBLIC STATIC void vEthernetTask(void * pvArg);

#endif // __W5500CLASS_H__
