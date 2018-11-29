#include "w5500klass.h"

#include "socket.h"

#define NUM_OF_SPI_PINS	((uint8_t)4)
#define SPI_NSS_PIN		((uint8_t)0)
#define SPI_SCK_PIN		((uint8_t)1)
#define SPI_MISO_PIN	((uint8_t)2)
#define SPI_MOSI_PIN	((uint8_t)3)

#define BUFFER_SIZE     ((uint8_t)32)

// private method
PRIVATE void initW5500Base(W5500Class * this);
PRIVATE void syncW5500(W5500Class * this);
PRIVATE void w5500Receive(W5500Class * this);
PRIVATE void w5500Send(W5500Class * this);

// private static method
PRIVATE STATIC void initW5500SPI(SPI_TypeDef * spi);
PRIVATE STATIC void initW5500SPIPins(SPI_TypeDef * spi);
PRIVATE STATIC void setW5500SPIClock(SPI_TypeDef * spi, FunctionalState newState);

PUBLIC W5500Class * newW5500Class(SPI_TypeDef * spi) {
	W5500Class * w5500ptr = malloc(sizeof(W5500Class));

	if (w5500ptr == NULL) {
		return NULL;
	}

	initW5500Base(w5500ptr);

	w5500ptr->crisEnter = NULL;
	w5500ptr->crisExit = NULL;
	w5500ptr->csSelect = NULL;
	w5500ptr->csDeselect = NULL;
	w5500ptr->spiReadByte = NULL;
	w5500ptr->spiWriteByte = NULL;

	initW5500SPI(spi);
	
	return w5500ptr;
}

PRIVATE void initW5500Base(W5500Class * this) {
	this->base._rxBuffer = NULL;
	this->base._rxBufferSemphr = xSemaphoreCreateMutex();
	this->base._rxBufferSize = 0;
	this->base._numOfRXData = 0;
	
	this->base._txBuffer = NULL;
	this->base._txBufferSemphr = xSemaphoreCreateMutex();
	this->base._txBufferSize = 0;
	this->base._numOfTXData = 0;
	
	this->base.init = initW5500;
	this->base.onReceive = onW5500Receive;
	this->base.sendMessage = sendW5500Message;
}

PRIVATE void syncW5500(W5500Class * this) {
	__RELOAD_WATCHDOG();

	w5500Receive(this);
	w5500Send(this);
}

PRIVATE void w5500Receive(W5500Class * this) {
	uint16_t numOfRXData = getSn_RX_RSR(ETHERNET_SOCK_TCPS);

	if(numOfRXData > 0 && this->base._numOfRXData == 0 
            && xSemaphoreTake(this->base._rxBufferSemphr, 0) == pdTRUE) {
		if (numOfRXData > this->base._rxBufferSize) {
			numOfRXData = this->base._rxBufferSize;
		}
		
		numOfRXData = recv(ETHERNET_SOCK_TCPS, this->base._rxBuffer, numOfRXData);
		if (numOfRXData > 0) {
			this->base._numOfRXData = numOfRXData;
		}
        
        xSemaphoreGive(this->base._rxBufferSemphr);
	}
}

PRIVATE void w5500Send(W5500Class * this) {
	if (this->base._numOfTXData > 0 
			&& xSemaphoreTake(this->base._txBufferSemphr, 0) == pdTRUE) {
		if (send(ETHERNET_SOCK_TCPS, this->base._txBuffer, this->base._numOfTXData) < 0) {
			close(ETHERNET_SOCK_TCPS);
            
			xSemaphoreGive(this->base._txBufferSemphr);
			return;
		}
		
		this->base._numOfTXData = 0;
		xSemaphoreGive(this->base._txBufferSemphr);
	}
}

PUBLIC VIRTUAL bool initW5500(struct Ethernet * this) {
	W5500Class * w5500ptr = (W5500Class *)this;
	
	reg_wizchip_cris_cbfunc(w5500ptr->crisEnter, w5500ptr->crisExit);
	reg_wizchip_cs_cbfunc(w5500ptr->csSelect, w5500ptr->csDeselect);
	reg_wizchip_spi_cbfunc(w5500ptr->spiReadByte, w5500ptr->spiWriteByte);
	
	// WIZCHIP SOCKET Buffer initialize
	uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};
	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1) {
		return  false;
	}
	
	wiz_NetInfo netInfo = {.dhcp = NETINFO_STATIC};
	memcpy(netInfo.mac, ETHERNET_CLIENT_MAC, ETHERNET_MAC_LEN * sizeof(uint8_t));
	memcpy(netInfo.ip, this->_clientIP, ETHERNET_IP_LEN * sizeof(uint8_t));
	memcpy(netInfo.sn, this->_subnetMask, ETHERNET_IP_LEN * sizeof(uint8_t));
	memcpy(netInfo.gw, this->_gatewayIP, ETHERNET_IP_LEN * sizeof(uint8_t));
	ctlnetwork(CN_SET_NETINFO, (void*)&netInfo);

#ifndef DEBUG
	wiz_NetTimeout netTimeOut = {.retry_cnt = 3, .time_100us = 1000};
	wizchip_settimeout(&netTimeOut);
#endif
	
	return true;
}

PUBLIC VIRTUAL void onW5500Receive(struct Ethernet * this) {
    if (this->_numOfRXData > 0 && xSemaphoreTake(this->_rxBufferSemphr, 0) == pdTRUE) {
        uint8_t * message = this->_rxBuffer;
        uint8_t msgLength = this->_numOfRXData;
        
        // check message head
        if (message[0] != NETMSG_HEAD) {
            this->_numOfRXData = 0;
            xSemaphoreGive(this->_rxBufferSemphr);
            
            return;
        }
		
        if (deEthernetTransparentTrans(message, msgLength)) {
			handleEthernetMessage(message);
        }
    }
    
    this->_numOfRXData = 0;
    xSemaphoreGive(this->_rxBufferSemphr);
}

PUBLIC VIRTUAL uint8_t sendW5500Message(struct Ethernet * this, 
		uint8_t type, EthernetObject objects[], uint8_t numOfObjects) {
	uint8_t length = 0;
	uint8_t * message = this->_txBuffer;
	bool getSemphr = xSemaphoreTake(this->_txBufferSemphr, 500 / portTICK_RATE_MS);

	if (message == NULL || this->_numOfTXData != 0 || !getSemphr) {
		return 0;
	}

	uint8_t linkCode = 0x00;
    switch (type) {
        case NETMSG_FORWARDLY_REPORT_TYPE:
            linkCode = NETMSG_INFORMATION_LINK_CODE;
            break;
    }
        
	// add ethernet message head
	__ADD_NETMSG_HEAD(message, length);
	__ADD_NETMSG_CONTENT(message, length, NETMSG_VERSION_NUMBER);
	__ADD_NETMSG_CONTENT(message, length, NETMSG_CONTROLLER_SENDER_ID);
	__ADD_NETMSG_CONTENT(message, length, NETMSG_SERVER_RECEIVER_ID);
	__ADD_NETMSG_CONTENT(message, length, linkCode);
	__ADD_NETMSG_CONTENT(message, length, NETMSG_DISTRICT_NUMBER);
	__ADD_NETMSG_CONTENT(message, length, NETMSG_INTERSECTION_NUMBER);
	__ADD_NETMSG_CONTENT(message, length, (0x80 | (numOfObjects - 1) << 4 | type));

	// add ethernet message objects
	for (uint8_t i = 0; i < numOfObjects; i++) {
		__ADD_NETMSG_CONTENT(message, length, objects[i].type);
		__ADD_NETMSG_CONTENT(message, length, objects[i].numOfIndexes << 6);
		for (uint8_t j = 0; j < objects[i].numOfIndexes; j++) {
			__ADD_NETMSG_CONTENT(message, length, objects[i].index[j]);
		}
            
		__ADD_NETMSG_CONTENT(message, length, objects[i].value[0]);
	}
		
	// add ethernet message tail
	__ADD_NETMSG_CHECKCODE(message, length);
	__ADD_NETMSG_TAIL(message, length);

	this->_numOfTXData = length;
	xSemaphoreGive(this->_txBufferSemphr);
	return length;
}

PRIVATE STATIC void initW5500SPI(SPI_TypeDef * spi) {
	initW5500SPIPins(spi);
	setW5500SPIClock(spi, ENABLE);
	
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
	
	SPI_Init(spi, &SPI_InitStructure);
	SPI_SSOutputCmd(spi, ENABLE);
	SPI_Cmd(spi, ENABLE);
}

PRIVATE STATIC void initW5500SPIPins(SPI_TypeDef * spi) {
	GPIOPin pins[NUM_OF_SPI_PINS];
	
	if (spi == SPI1) {
		pins[SPI_NSS_PIN] = newGPIOPin(GPIOA, GPIO_Pin_4);
		pins[SPI_SCK_PIN] = newGPIOPin(GPIOA, GPIO_Pin_5);
		pins[SPI_MISO_PIN] = newGPIOPin(GPIOA, GPIO_Pin_6);
		pins[SPI_MOSI_PIN] = newGPIOPin(GPIOA, GPIO_Pin_7);
	} else {
		if (spi == SPI2) {
			pins[SPI_NSS_PIN] = newGPIOPin(GPIOB, GPIO_Pin_12);
			pins[SPI_SCK_PIN] = newGPIOPin(GPIOB, GPIO_Pin_13);
			pins[SPI_MISO_PIN] = newGPIOPin(GPIOB, GPIO_Pin_14);
			pins[SPI_MOSI_PIN] = newGPIOPin(GPIOB, GPIO_Pin_15);
		}
	}
	
	for (uint8_t i = 0; i < NUM_OF_SPI_PINS; i++) {
		switch (i) {
			case SPI_NSS_PIN:
				configGPIOPin(pins + i, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
				GPIO_SetBits(pins[i]._port, pins[i]._pin);
				break;

			case SPI_SCK_PIN:
			case SPI_MISO_PIN:
			case SPI_MOSI_PIN:
				configGPIOPin(pins + i, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);
				break;
		}
	}
}

PRIVATE STATIC void setW5500SPIClock(SPI_TypeDef * spi, FunctionalState newState) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	if (spi == SPI1) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, newState);
	} else {
		if (spi == SPI2) {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, newState);
		}
	}
}

PUBLIC STATIC void vEthernetTask(void * pvArg) {
	W5500Class * w5500ptr = pvArg;
	uint8_t txBuffer[BUFFER_SIZE], rxBuffer[BUFFER_SIZE];
	setEthernetRXBuffer((struct Ethernet *)w5500ptr, rxBuffer, BUFFER_SIZE);
	setEthernetTXBuffer((struct Ethernet *)w5500ptr, txBuffer, BUFFER_SIZE);
	
	for (;;) {
		switch(getSn_SR(ETHERNET_SOCK_TCPS)) {
			case SOCK_ESTABLISHED:
				if(getSn_IR(ETHERNET_SOCK_TCPS) & Sn_IR_CON) {
					setSn_IR(ETHERNET_SOCK_TCPS, Sn_IR_CON);
				}
				syncW5500(w5500ptr);
				break;
				
			case SOCK_CLOSE_WAIT:
				disconnect(ETHERNET_SOCK_TCPS);
				break;
			
			case SOCK_INIT:
				connect(ETHERNET_SOCK_TCPS, w5500ptr->base._serverIP, 
                        w5500ptr->base._serverPort);
                xSemaphoreTake(w5500ptr->base._txBufferSemphr, portMAX_DELAY);
                w5500ptr->base._numOfTXData = 0;
                xSemaphoreGive(w5500ptr->base._txBufferSemphr);
				break;
			
			case SOCK_CLOSED:
				close(ETHERNET_SOCK_TCPS);
				socket(ETHERNET_SOCK_TCPS, Sn_MR_TCP, ETHERNET_CLIENT_PORT, 0x00);
				break;
			
			default:
				break;
		}
        
        w5500ptr->base.onReceive((struct Ethernet *)w5500ptr);
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}
