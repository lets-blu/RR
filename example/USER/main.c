#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

#include "controller.h"
#include "spi.h"
#include "w5500klass.h"
#include "watchdog.h"

#define configWatchDog()    staticWatchDog()
#define configController()  staticController()

void configControllerManual(void);
void configControllerEthernet(void);
void configControllerDirection(void);

void createStartUpTask(void);

int main(void)
{
#ifndef DEBUG
    configWatchDog();
#endif
    
	configController();
    configControllerManual();
    configControllerEthernet();
    configControllerDirection();
	
	createStartUpTask();
	vTaskStartScheduler();
	
	deleteDataSelector(&dataSelector1);
    for (uint8_t i = 0; i < controller->_numOfDirections; i++) {
        deleteDirection(controller->_directions[i]);
    }
    deleteController(controller);
}

void configControllerManual(void) {
	setControllerManual(controller, newManual(
			(LED *[]){
				DEFAULT_LEFTTURN_LED	, 
				DEFAULT_STRAIGHT_LED	, 
				DEFAULT_ALL_RED_LED		, 
				DEFAULT_EAST_WEST_LED	, 
				DEFAULT_SOUTH_NORTH_LED
			}, 
			(Button *[]){
				DEFAULT_LAMP_BUTTON		, 
				DEFAULT_ROAD_BUTTON		, 
				DEFAULT_OK_BUTTON		, 
				DEFAULT_QUIT_BUTTON
			}
		)
	);
}

void configControllerEthernet(void) {
    setControllerEthernet(controller, (struct Ethernet *)newW5500Class(SPI2));
	setEthernetServerParams(controller->_ethernet, 
			(uint8_t []){192, 168, 1, 107}, 60000);
	setEthernetClientParams(controller->_ethernet, (uint8_t []){192, 168, 1, 199}, 
            (uint8_t []){255, 255, 255, 0}, (uint8_t []){192, 168, 1, 1});
    
	((W5500Class *)controller->_ethernet)->crisEnter = SPI_CrisEnter;
	((W5500Class *)controller->_ethernet)->crisExit = SPI_CrisExit;
	((W5500Class *)controller->_ethernet)->csSelect = SPI_CS_Select;
	((W5500Class *)controller->_ethernet)->csDeselect = SPI_CS_Deselect;
	((W5500Class *)controller->_ethernet)->spiReadByte = SPI_ReadByte;
	((W5500Class *)controller->_ethernet)->spiWriteByte = SPI_WriteByte;
    
	controller->_ethernet->init(controller->_ethernet);
}

void configControllerDirection(void) {
	Direction * eastWestStraight = newDirection(EASTWEST_STRAIGHT_STRING, 
            newLamp(DEFAULT_REGISTER_GROUP, 0, 1, 2), 0);
    Direction * southNorthStraight = newDirection(SOUTHNORTH_STRAIGHT_STRING, 
            newLamp(DEFAULT_REGISTER_GROUP, 3, 4, 5), 0);
    // Direction * eastWestLeftTurn = newDirection(EASTWEST_LEFTTURN_STRING, 
    //         newLamp(DEFAULT_REGISTER_GROUP, 3, 4, 5), 0);
    // Direction * southNorthLeftTurn = newDirection(SOUTHNORTH_LEFTTURN_STRING, 
    //         newLamp(DEFAULT_REGISTER_GROUP, 9, 10, 11), 0);

    // addDetectorToDirection(eastWestStraight, newDetector(0, 1), &dataSelector1);
    // addDetectorToDirection(southNorthStraight, newDetector(7, 1), &detectorGroup1);
    // addDetectorToDirection(eastWestStraight, newDetector(0, 1), &dataSelector1);
    
	addDirectionToController(controller, eastWestStraight);
    addDirectionToController(controller, southNorthStraight);
    // addDirectionToController(controller, eastWestLeftTurn);
    // addDirectionToController(controller, southNorthLeftTurn);
}

void createStartUpTask(void) {
	xTaskCreate(vAutoRunTask, "AutoRunTask", configMINIMAL_STACK_SIZE, 
            controller, 3, &xAutoRunTask);
	xTaskCreate(vHandleManualTask, "HandleManualTask", configMINIMAL_STACK_SIZE, 
			controller, 2, NULL);
	xTaskCreate(vScanDataSelectorTask, "ScanDataSelectorTask", configMINIMAL_STACK_SIZE, 
			DEFAULT_DATA_SELECTOR, 1, NULL);
	xTaskCreate(vHandleSelectorMessagesTask, "HandleDataSelectorMessagesTask", configMINIMAL_STACK_SIZE, 
			DEFAULT_DATA_SELECTOR, 1, NULL);
	xTaskCreate(vEthernetTask, "EthernetTask", configMINIMAL_STACK_SIZE, 
            controller->_ethernet, 1, NULL);
}
