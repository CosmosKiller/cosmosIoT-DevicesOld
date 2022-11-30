/**
 * Custom library for std CosmosIOT devices mangement
 */

#ifndef MAIN_STDCOSMOSDEV_H_
#define MAIN_STDCOSMOSDEV_H_

#include "CosmosClient.h"

/**
 * @brief This function is used to controll the state (on/off)
 * of a certain socket
 *
 * @param snValue Serial number used to compare and
 * determine with which socket we are trying to interact
 * @param qty Quantity of devices used in the project
 * @param dev[] Devices strutct that contains all of the info
 * about the devices used in the project
 */
const char *socketControll(String snValue, int qty, Devices_t dev[]);

/**
* @brief This function is used to controll color and brightness of
* any RGB ligth source

* @param snValue Serial number used to compare and
* determine with which ligth source we are trying to interact
* @param rgbValues An string that contains Brigthness and RGB values.
* Each of them must be in the following order -> "Bright/Rvalue/Gvalue/Bvalue/" e.g. 050/255/112/017/
* @param qty Quantity of devices used in the project
* @param dev[] Devices strutct that contains all of the info
* about the devices used in the project
*/
const char *lightControll(String snValue, String rgbValues, int qty, Devices_t dev[]);

/**
 * @brief Call this function whenever you need/want to
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

#endif /* MAIN_STDCOSMOSDEV_H_ */