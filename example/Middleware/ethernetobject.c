#include "ethernetobject.h"

#include "controller.h"

// private static method(s)
PRIVATE STATIC void handleTimingSchemeObject(EthernetObject object);
PRIVATE STATIC void handleModeSettingObject(EthernetObject object);

PUBLIC STATIC void handleEthernetObject(EthernetObject object) {
    switch (object.type) {
    case NETOBJ_TIMING_SCHEME_TYPE:
        handleTimingSchemeObject(object);
        break;

    case NETOBJ_MODE_SETTING_TYPE:
        handleModeSettingObject(object);
        break;
    }
}

PRIVATE STATIC void handleTimingSchemeObject(EthernetObject object) {
    const char * name = ALLRED_STRING;
    
    switch (object.index[0]) {
    case ALLRED_INDEX:
        if (object.index[1] == ALLRED_INDEX) {
            name = ALLRED_STRING;
        }
        break;

    case EASTWEST_INDEX:
        if (object.index[1] == STRAIGHT_INDEX) {
            name = EASTWEST_STRAIGHT_STRING;
        } else {
            if (object.index[1] == LEFTTURN_INDEX) {
                name = EASTWEST_LEFTTURN_STRING;
            }
        }
        break;

    case SOUTHNORTH_INDEX:
        if (object.index[1] == STRAIGHT_INDEX) {
            name = SOUTHNORTH_STRAIGHT_STRING;
        } else {
            if (object.index[1] == LEFTTURN_INDEX) {
                name = SOUTHNORTH_LEFTTURN_STRING;
            }
        }
        break;
    }
    
    xQueueSendToBack(controller->_xManualQueue, &name, 0);
}

PRIVATE STATIC void handleModeSettingObject(EthernetObject object) {
    if (object.value[0] == 0x00) {
        const char * name = QUIT_STRING;
        
        xQueueSendToBack(controller->_xManualQueue, &name, 0);
    }
}
