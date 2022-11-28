/**
* Standard library used to manage all the I/O functionalities, reciving 
* the commands via the serial pins (TX0/RX0)
* It can also be used to return information such as temperature, humidity and
* polution messurements, etc.
*/ 

#ifndef MAIN_COSMOSSERIALCOM_H_
#define MAIN_COSMOSSERIALCOM_H_

#include "Arduino.h"

/**
 * @brief Standard struct for serial comunication
 * between cosmosIoT devices
 * 
 */
typedef struct cosmosSerialCom
{
   String msg;
   String sn;
   String category;
} Payload_t;

/**
* This function is meant to be used it within the callback()
* that we'll define and use in the setCallback() method.
*
* @param payload Message sent by the mqtt broker
* @param length The length of the payload
* @return An string called msg, that we'll use in callback() 
* function. Its application will depend on the needs of
* our project
*/
String payloadCapture(byte* payload, unsigned int length);

/**
* Use this function as the parameter for cosmosMqttSetup()
* method if you are sending the message recived from
* the broker, to another MCU via serial pins.
* Last two parameters are the same for payloadCapture()
*
* @param topic Topic in which the mqtt broker publishes the message
* @param payload Message sent by the mqtt broker
* @param length The length of the payload
*/
void payloadSerialSend(char* topic, byte* payload, unsigned int length);

/**
* Using a custom struct we can split and store 
* the string recived through the serial pins.
*
* @param incomingPayload String recived via serial pins
* @return Struct that contains three different Strings (sn, msg, category)
* which we'll use in our code depending on the project needs
*/
Payload_t payloadSerialSplit(String incomingPayload);

#endif /* MAIN_COSMOSSERIALCOM_H_ */