/**
 * WiFi cliente connection and MQTT broker access
 */

#ifndef MAIN_COSMOSCLIENT_H_
#define MAIN_COSMOSCLIENT_H_

#include "Arduino.h"
#include "PubSubClient.h"

#ifdef SNRTH_PIN
#include "DHTesp.h"

extern DHTesp myDHT;
#endif

#define LSC 1
#define SKT 2
#define SNR 3
#define CAM 4
#define MOT 5

#define RX_CONTROL "/rx_controll"
#define RX_STATE "/rx_state"

/**
 * Standard structure for devices management
 * DON'T MODIFY THIS STRUCT!
 */
typedef struct CosmosIOT
{
    /* Serial Number of the devices */
    const String sn;
    /* Pins in which the device will be connected
     *  For LED -> {rPin, gPin, bPin}
     *  For other -> {devPin, 0, 0}
     */
    const int pin[3];
    /* Initial state of the device (HIGH/LOW) */
    int state;
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
 * @brief Client connection and persistance to the server
 *
 * @param qty Quantity of devices used in the project
 * @param dev[] Devices strutct that contains all of the info
 * about the devices used in the project
 */
void cosmosMqttLoop(int qty, Devices_t dev[]);

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

#endif /* MAIN_COSMOSCLIENT_H_ */