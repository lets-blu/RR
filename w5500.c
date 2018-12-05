#include "w5500.h"

#include "socket.h"

#define __SOCK_TCP ((uint8_t)0)

// private method
PRIVATE void w5500Send(W5500 * this);
PRIVATE void w5500Receive(W5500 * this);

PUBLIC W5500 newW5500(SPI_TypeDef * spi) {
	W5500 w5500 = {
		.base		= newEthernet(), 
		._port		= newSPIPort(spi), 
		.cris_en	= NULL, 
		.cris_ex	= NULL, 
		.cs_sel		= NULL, 
		.cs_desel	= NULL, 
		.spi_rb		= NULL, 
		.spi_wb		= NULL
	};
	
	w5500.base.init = (bool (*)(struct Ethernet *))initW5500;
	
	initSPIPort(&w5500._port);
	
	return w5500;
}

PUBLIC void syncW5500(W5500 * this) {
	switch(getSn_SR(__SOCK_TCP)) {
		case SOCK_ESTABLISHED:
			if(getSn_IR(__SOCK_TCP) & Sn_IR_CON) {
				setSn_IR(__SOCK_TCP, Sn_IR_CON);
			}
			
			w5500Send(this);
			w5500Receive(this);
			break;
			
		case SOCK_CLOSE_WAIT:
			disconnect(__SOCK_TCP);
			break;
		
		case SOCK_INIT:
			connect(__SOCK_TCP, 
					this->base._serverIP, this->base._serverPort);
			break;
		
		case SOCK_CLOSED:
			close(__SOCK_TCP);
			socket(__SOCK_TCP, Sn_MR_TCP, this->base._clientPort, 0x00);
			break;
		
		default:
			break;
	}
}

 PUBLIC VIRTUAL bool initW5500(W5500 * this) {
 	reg_wizchip_cris_cbfunc(this->cris_en, this->cris_ex);
 	reg_wizchip_cs_cbfunc(this->cs_sel, this->cs_desel);
 	reg_wizchip_spi_cbfunc(this->spi_rb, this->spi_wb);
	
 	// WIZCHIP SOCKET Buffer initialize
 	uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};
 	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1) {
 		return  false;
 	}
	
 	wiz_NetInfo netInfo = {.dhcp = NETINFO_STATIC};
 	memcpy(netInfo.mac, ETHERNET_CLIENT_MAC, ETHERNET_MAC_LEN * sizeof(uint8_t));
 	memcpy(netInfo.ip, this->base._clientIP, ETHERNET_IP_LEN * sizeof(uint8_t));
 	memcpy(netInfo.sn, this->base._subnetMask, ETHERNET_IP_LEN * sizeof(uint8_t));
 	memcpy(netInfo.gw, this->base._gatewayIP, ETHERNET_IP_LEN * sizeof(uint8_t));
 	ctlnetwork(CN_SET_NETINFO, (void*)&netInfo);

 #ifndef DEBUG
 	wiz_NetTimeout netTimeOut = {.retry_cnt = 3, .time_100us = 1000};
 	wizchip_settimeout(&netTimeOut);
 #endif
	
 	return true;
}

PRIVATE void w5500Send(W5500 * this) {
	if (!(((struct Ethernet *)this)->_txBuffer) 
			|| !(((struct Ethernet *)this)->_numOfTxData)) {
		return;
	}

	if (send(__SOCK_TCP, this->base._txBuffer, this->base._numOfTxData) < 0) {
		close(__SOCK_TCP);
		return;
	}

	((struct Ethernet *)this)->_numOfTxData = 0;
}

PRIVATE void w5500Receive(W5500 * this) {
	uint16_t length = getSn_RX_RSR(__SOCK_TCP);
	if (!length || !(((struct Ethernet *)this)->_rxBuffer)
			|| ((struct Ethernet *)this)->_numOfRxData) {
		return;
	}
	
	if (length > ((struct Ethernet *)this)->_rxBufferSize) {
		length = ((struct Ethernet *)this)->_rxBufferSize;
	}
	
	if (recv(__SOCK_TCP, ((struct Ethernet *)this)->_rxBuffer, length) > 0) {
		((struct Ethernet *)this)->_numOfRxData = length;
		((struct Ethernet *)this)->onReceive((struct Ethernet *)this);
	}
}
