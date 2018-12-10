#include "ethernet.h"

PUBLIC struct Ethernet newEthernet(void) {
    struct Ethernet ethernet = {
        ._serverPort    = 0, 
        ._serverIP      = {0, 0, 0, 0}, 

        ._clientPort    = 0, 
        ._clientIP      = {0, 0, 0, 0}, 

        ._subnetMask    = {0, 0, 0, 0}, 
        ._gatewayIP     = {0, 0, 0, 0}, 

        ._rxBuffer      = NULL, 
        ._rxBufferSize  = 0, 
        ._numOfRxData   = 0, 

        ._txBuffer      = NULL, 
        ._txBufferSize  = 0, 
        ._numOfTxData   = 0, 

        .init           = defaultEthernetInit, 
        .send           = defaultEthernetSend, 
        .onReceive      = defaultOnEthernetReceive
    };

    return ethernet;
}

PUBLIC void setEthernetServerParams(struct Ethernet * this, 
		uint8_t serverIP[ETHERNET_IP_LEN], uint16_t serverPort) {
	memcpy(this->_serverIP, serverIP, ETHERNET_IP_LEN * sizeof(uint8_t));
	this->_serverPort = serverPort;
}

PUBLIC void setEthernetClientParams(struct Ethernet * this, 
		uint8_t clientIP[], uint16_t clientPort) {
	memcpy(this->_clientIP, clientIP, ETHERNET_IP_LEN * sizeof(uint8_t));
	this->_clientPort = clientPort;
}

PUBLIC void setEthernetNetworkParams(struct Ethernet * this, 
		uint8_t subnetMask[], uint8_t gatewayIP[]) {
	memcpy(this->_subnetMask, subnetMask, ETHERNET_IP_LEN * sizeof(uint8_t));
	memcpy(this->_gatewayIP, gatewayIP, ETHERNET_IP_LEN * sizeof(uint8_t));
}

PUBLIC void setEthernetRxBuffer(struct Ethernet * this, 
		uint8_t * rxBuffer, uint8_t rxBufferSize) {
	this->_rxBuffer = rxBuffer;
	this->_rxBufferSize = rxBufferSize;
	this->_numOfRxData = 0;
}

PUBLIC void setEthernetTxBuffer(struct Ethernet * this, 
		uint8_t * txBuffer, uint8_t txBufferSize) {
	this->_txBuffer = txBuffer;
	this->_txBufferSize = txBufferSize;
	this->_numOfTxData = 0;
}
				
PUBLIC VIRTUAL bool defaultEthernetInit(struct Ethernet * this) {
	return false;
}

PUBLIC VIRTUAL uint8_t defaultEthernetSend(struct Ethernet * this, 
		uint8_t message[], uint8_t length) {
	if (this->_txBuffer == NULL || this->_numOfTxData > 0) {
		return 0;
	}
	
	memcpy(this->_txBuffer, message, length * sizeof(uint8_t));
	this->_numOfTxData = length;
	
	return length;
}

PUBLIC VIRTUAL void defaultOnEthernetReceive(struct Ethernet * this) {
	
}
