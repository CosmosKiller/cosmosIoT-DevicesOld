#ifndef MAIN_MQTTCUSTOMFUNC_H_
#define MAIN_MQTTCUSTOMFUNC_H_

#include "Arduino.h"

typedef struct MqttCustomFunc
{
   String msg;
   String sn;
   String category;
} Payload_t;

/**
* This functions returns an String that contains the topic and-sub topic that
* the device is going to suscribe to.
*
* @param serialNumber Use the member sn of the "Devices" struct in order to pass the
* serial number as a paramter
* @param aux A simple counter, use 0 as default value when declaring it
* @return Notice that 'topic' is returned as an String, you need to 
* cast it to a char array
*/
String topicGeneration(String serialNumber, int aux);

/**
* This function is the meant to use it within the callback()
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
* Use this function as the parameter for setCallback()
* method is you are sending the messagerecived from
* the broker, to another MCU via serial pins.
* Last two parameters are the same for payloadCapture()
*
* @param topic Topic in which the mqtt broker publishes the message
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
Payload_t payloadSplit(String incomingPayload);

#endif /* MAIN_MQTTCUSTOMFUNC_H_ */