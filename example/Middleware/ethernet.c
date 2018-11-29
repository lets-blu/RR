#include "ethernet.h"

#define MESSAGE_VERSION_NUMBER              ((uint8_t)1)
#define MESSAGE_SENDER_ID                   ((uint8_t)2)
#define MESSAGE_RECEIVER_ID                 ((uint8_t)3)
#define MESSAGE_LINK_CODE                   ((uint8_t)4)
#define MESSAGE_DISTRICT_NUMBER             ((uint8_t)5)
#define MESSAGE_INTERSECTION_NUMBER         ((uint8_t)6)
#define MESSAGE_NUM_OF_OBJECTS              ((uint8_t)7)

#define __NUM_OF_OBJECTS(message) {                         \
    (((message[MESSAGE_NUM_OF_OBJECTS] & 0x70) >> 4) + 1)   \
}

#define __NUM_OF_INDEXES(message, offset) {                 \
    (message[offset] >> 6)                                  \
}

PUBLIC void setEthernetServerParams(struct Ethernet * this, 
		uint8_t serverIP[ETHERNET_IP_LEN], uint16_t serverPort) {
	memcpy(this->_serverIP, serverIP, ETHERNET_IP_LEN * sizeof(uint8_t));
	this->_serverPort = serverPort;
}

PUBLIC void setEthernetClientParams(struct Ethernet * this, 
		uint8_t clientIP[ETHERNET_IP_LEN], uint8_t subnetMask[ETHERNET_IP_LEN], 
		uint8_t gatewayIP[ETHERNET_IP_LEN]) {
	memcpy(this->_clientIP, clientIP, ETHERNET_IP_LEN * sizeof(uint8_t));
	memcpy(this->_subnetMask, subnetMask, ETHERNET_IP_LEN * sizeof(uint8_t));
	memcpy(this->_gatewayIP, gatewayIP, ETHERNET_IP_LEN * sizeof(uint8_t));
}


PUBLIC void setEthernetRXBuffer(struct Ethernet * this, 
        uint8_t * rxBuffer, uint8_t rxBufferSize) {
	this->_rxBuffer = rxBuffer;
	this->_rxBufferSize = rxBufferSize;
	this->_numOfRXData = 0;
}

PUBLIC void setEthernetTXBuffer(struct Ethernet * this, 
        uint8_t * txBuffer, uint8_t txBufferSize) {
	this->_txBuffer = txBuffer;
	this->_txBufferSize = txBufferSize;
	this->_numOfTXData = 0;
}

PUBLIC STATIC void handleEthernetMessage(uint8_t * message) {
    if (message[MESSAGE_VERSION_NUMBER] != NETMSG_VERSION_NUMBER 
            || message[MESSAGE_DISTRICT_NUMBER] != NETMSG_DISTRICT_NUMBER 
            || message[MESSAGE_INTERSECTION_NUMBER] != NETMSG_INTERSECTION_NUMBER) {
        return;
    }
    
    uint8_t numOfObjects = __NUM_OF_OBJECTS(message);
    uint8_t offset = MESSAGE_NUM_OF_OBJECTS + 1;
    for (uint8_t i = 0; i < numOfObjects; i++) {
        EthernetObject object = {
            .type           = message[offset++], 
            .numOfIndexes   = __NUM_OF_INDEXES(message, offset++)
        };
        
        if (object.numOfIndexes > NETOBJ_NUM_OF_INDEXES) {
            return;
        }
        
        for (uint8_t j = 0; j < object.numOfIndexes; j++) {
            object.index[j] = message[offset++];
        }
        
        object.value[0] = message[offset++];
        handleEthernetObject(object);
    }
}

PUBLIC STATIC bool deEthernetTransparentTrans(uint8_t * message, uint8_t length) {
    uint8_t i = 1;

    for (; i < length && message[i] != NETMSG_TAIL; i++) {
        if (message[i] != 0xdb) {
            continue;
        }

        if (message[i + 1] == 0xdc || message[i + 1] == 0xdd) {
            for (uint8_t j = i; j < length; j++) {
                message[j] = message[j + 1];
            }

            message[i] = message[i] == 0xdc ? 0xc0 : 0xdb;
            length--;
        } else {
            break;
        }
    }

    return i == length - 1;
}
