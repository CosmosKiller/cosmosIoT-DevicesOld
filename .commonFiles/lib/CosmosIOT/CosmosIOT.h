/**
* Custom library for all Cosmos-IoT platform projects
*/

#ifndef MAIN_COSMOSIOT_H_
#define MAIN_COSMOSIOT_H_

#include "Arduino.h"
#include "PubSubClient.h"

#include "MqttCustomFunc.h"

#define LSC             1
#define SKT             2
#define SNR             3
#define CAM             4
#define MOT             5

#define RX_CONTROL  "/rx_controll"
#define RX_STATE    "/rx_state"

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
* @brief Use this function within the setup() in order
* to configure pin modes and initial values.
*
* @param qty Quantity of devices used in the project
* @param dev[] Devices strutct that contains all of the info
* about the devices used in the project
*/
void cosmosBegin(int qty, Devices_t dev[]);

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
String socketControll(String snValue, int qty, Devices_t dev[]);

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
String lightControll(String snValue, String rgbValues, int qty, Devices_t dev[]);

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

/**
 * @brief Client connection and persistance to the server
 * 
* @param qty Quantity of devices used in the project
* @param dev[] Devices strutct that contains all of the info
* about the devices used in the project
*/
void cosmosMqttLoop (int qty, Devices_t dev[]);

/**
 * @brief Mqtt client and wifi setup
 * 
 * @param callback Function must recive the following params:
 *  ->char* topic ; 
 *  ->byte* payload ; 
 *  ->unsigned int length
 * @param board Choose between CIOT_ESP32 or CIOT_ESP8266
 */
void cosmosMqttSetup(MQTT_CALLBACK_SIGNATURE);

/**
 * @brief Mqtt client publishing
 * 
 * @param msg Message sent to the mqtt broker
 * @param devSerial Device serial number -> device[i].sn
 * @param topic Topic in which the mqtt broker publishes the message
 */
void cosmosMqttPublish(const char *msg, String devSerial, const char *topic);

/**
 * @brief Wifi setup funcion. 
 * 
 */
void cosmosWifiSetup(void);

#endif /* MAIN_COSMOSIOT_H_ */