/**
* Custom library for all Cosmos-IoT platform projects
*/

#ifndef MAIN_COSMOSIOT_H_
#define MAIN_COSMOSIOT_H_

#include "Arduino.h"

/**
* Standard structure for devices management
* DON'T MODIFY THIS STRUCT!
*/
typedef struct CosmosIOT {
const String sn; //Serial Number of the devices
const int pin[3]; //Pin in which the device will be connected
int state; //Initial state of the device (HIGH/LOW)
} Devices_t;

/**
* Use this function within the setup() in order
* to configure pin modes and initial values.
*
* @param qty Quantity of devices used in the project
* @param dev[] Devices strutct that contains all of the info
* about the devices used in the project
*/
void cosmosBegin(int qty, Devices_t dev[]);

/**
* This function is used to controll the state (on/off)
* of a certain socket
*
* @param snValue Serial number used to compare and 
* determine with which socket we are trying to interact
* @param qty Quantity of devices used in the project
* @param dev[] Devices strutct that contains all of the info
* about the devices used in the project
*/
String socketControll(String snValue, int qty, Devices_t dev[]);

/**
* This function is used to controll color and brightness of
* any RGB ligth source

* @param snValue Serial number used to compare and
* determine with which ligth source we are trying to interact
* @param rgbValues An string that contains Brigthness and RGB values.
* Each of them must be in the following order -> "Bright/Rvalue/Gvalue/Bvalue/" e.g. 050/255/112/017/
* @param qty Quantity of devices used in the project
* @param dev[] Devices strutct that contains all of the info
* about the devices used in the project
*/
String lightControll(String snValue, String rgbValues, int qty, Devices_t dev[]);

/**
* Call this function whenever you need/want to
* use buttons to locally control any device.
* 
* @param btnArray[] Pins in which the buttons are be connected 
* @param devArray[] Serial numbers used to compare and
* determine with which devices want to interact
* @param btnQty Quantity of buttons used in the project
* @param devQty Quantity of devices to be checked
* @param dev[] Devices strutct that contains all of the info
* about the devices used in the project
*/
void btnMonitor(int btnArray[], String devArray[], int btnQty, int devQty, Devices_t dev[]);

#endif /* MAIN_COSMOSIOT_H_ */