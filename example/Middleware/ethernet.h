#ifndef __ETHERNET_H__
#define __ETHERNET_H__

#include "FreeRTOS.h"
#include "semphr.h"
#include "stdbool.h"
#include "stm32f10x.h"
#include "string.h"

#include "ethernetobject.h"
#include "keyword.h"

#define ETHERNET_IP_LEN			        ((uint8_t)4)
#define ETHERNET_MAC_LEN		        ((uint8_t)6)

#define ETHERNET_SOCK_TCPS		        ((uint8_t)0)
#define ETHERNET_CLIENT_MAC 	        ((uint8_t []){0x00, 0x08, 0xdc, 0x00, 0xab, 0xcd})
#define ETHERNET_CLIENT_PORT	        ((uint16_t)50000)

#define NETMSG_HEAD                     ((uint8_t)0xc0)
#define NETMSG_VERSION_NUMBER           ((uint8_t)0x10)
#define NETMSG_CONTROLLER_SENDER_ID     ((uint8_t)0x10)
#define NETMSG_SERVER_RECEIVER_ID		((uint8_t)0x20)
#define NETMSG_RULES_LINK_CODE          ((uint8_t)0x01)
#define NETMSG_INFORMATION_LINK_CODE    ((uint8_t)0x02)
#define NETMSG_PARAMETERS_LINK_CODE     ((uint8_t)0x03)
#define NETMSG_INSTRUCTION_LINK_CODE    ((uint8_t)0x04)
#define NETMSG_DISTRICT_NUMBER          ((uint8_t)0x01)
#define NETMSG_INTERSECTION_NUMBER      ((uint8_t)0x01)
#define NETMSG_QUERY_REQUEST_TYPE       ((uint8_t)0x00)
#define NETMSG_SETTING_REAUEST_TYPE     ((uint8_t)0x02)
#define NETMSG_QUERY_RESPONSE_TYPE      ((uint8_t)0x04)
#define NETMSG_SETTING_RESPONSE_TYPE    ((uint8_t)0x05)
#define NETMSG_ERROR_RESPONSE_TYPE      ((uint8_t)0x06)
#define NETMSG_FORWARDLY_REPORT_TYPE    ((uint8_t)0x03)
#define NETMSG_TAIL                     ((uint8_t)0xc0)

#define __ADD_NETMSG_HEAD(buffer, offset) {				\
	buffer[offset++] = 0xc0;							\
}

#define __ADD_NETMSG_TAIL(buffer, offset) {				\
	__ADD_NETMSG_HEAD(buffer, offset);					\
}

#define __ADD_NETMSG_CONTENT(buffer, offset, value) {	\
	if (value == 0xc0) {								\
		buffer[offset++] = 0xdb;						\
		buffer[offset++] = 0xdc;						\
	} else if (value == 0xdb) {							\
		buffer[offset++] = 0xdb;						\
		buffer[offset++] = 0xdd;						\
	} else {											\
		buffer[offset++] = value;						\
	}													\
}

#define __ADD_NETMSG_CHECKCODE(buffer, offset) {		\
	__ADD_NETMSG_CONTENT(buffer, offset, 0x00);			\
}

struct Ethernet {
	uint16_t _serverPort;
	uint8_t _serverIP[ETHERNET_IP_LEN];
	
	uint8_t _clientIP[ETHERNET_IP_LEN];
	uint8_t _subnetMask[ETHERNET_IP_LEN], _gatewayIP[ETHERNET_IP_LEN];
	
	uint8_t * _rxBuffer;
	xSemaphoreHandle _rxBufferSemphr;
	uint8_t _rxBufferSize, _numOfRXData;
	
	uint8_t * _txBuffer;
	xSemaphoreHandle _txBufferSemphr;
	uint8_t _txBufferSize, _numOfTXData;
	
	bool (*init)(struct Ethernet * this);
	void (*onReceive)(struct Ethernet * this);
	uint8_t (*sendMessage)(struct Ethernet * this, uint8_t type, 
			EthernetObject objects[], uint8_t numOfObjects);
};

PUBLIC void setEthernetServerParams(struct Ethernet * this, 
		uint8_t serverIP[], uint16_t serverPort);
PUBLIC void setEthernetClientParams(struct Ethernet * this, 
		uint8_t clientIP[], uint8_t subnetMask[], uint8_t gatewayIP[]);
PUBLIC void setEthernetRXBuffer(struct Ethernet * this, 
		uint8_t * rxBuffer, uint8_t rxBufferSize);
PUBLIC void setEthernetTXBuffer(struct Ethernet * this, 
		uint8_t * txBuffer, uint8_t txBufferSize);

PUBLIC STATIC void handleEthernetMessage(uint8_t * message);
PUBLIC STATIC bool deEthernetTransparentTrans(uint8_t * message, uint8_t length);

#endif // __ETHERNET_H__
