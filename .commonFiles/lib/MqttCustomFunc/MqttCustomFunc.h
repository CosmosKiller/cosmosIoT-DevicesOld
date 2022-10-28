#include <stdio.h>
#include <Arduino.h>

struct MqttCustomFunc
{
   String msg;
   String sn;
   String category;
};

typedef struct MqttCustomFunc Payload;

/*
This functions returns an String that contains the topic and-sub topic that
the device is going to suscribe to.

@param serialNumber Use the member sn of the "Devices" struct in order to pass the
serial number as a paramter
@param aux A simple counter, use 0 as default value when declaring it
@return Notice that 'topic' is returned as an String, you need to 
cast it to a char array
*/
String topicGeneration(String serialNumber, int aux)
{
   String topic = "";
   String type = "" ;
   for (int j = 0 ; j < 14 ; j++) // 14 is the standar lenght of the serial numbers
   {
      if (j < 3)
         type += serialNumber[j];  
   }

   if (type == "HUB")
      topic = serialNumber + "/Homehubs";
   else if (type == "LSC")
      topic = serialNumber + "/Lights";
   else if (type == "SKT")
      topic = serialNumber + "/Sockets";
   else if (type == "SNR")
      topic = serialNumber + "/Sensors";
   else if (type == "CAM")
      topic = serialNumber + "/Cameras";

   topic.trim();
   return topic;
}

/*
This function is the meant to use it within the callback()
that we'll define and use in the setCallback() method.

@param payload Message sent by the mqtt broker
@param length The length of the payload
@return An string called msg, that we'll use in callback() 
function. Its application will depend on the needs of
our project
*/
String payloadCapture(byte* payload, unsigned int length) 
{
   String msg = "";
   for (unsigned int i = 0; i < length; i++)
      msg += (char)payload[i];

   msg.trim();

   return msg;
}

/*
Use this function as the parameter for setCallback()
method is you are sending the messagerecived from
the broker, to another MCU via serial pins.
Last two parameters are the same for payloadCapture()

@param topic Topic in which the mqtt broker publishes the message
*/
void payloadSerialSend(char* topic, byte* payload, unsigned int length)
{
   char toSend[50] = "";
   String msg = payloadCapture(payload, length);
   msg = msg + '|' + topic;
   msg.toCharArray(toSend, 50);

   Serial.write(toSend);
}

/*
Using a custom struct we can split and store 
the string recived through the serial pins.

@param incomingPayload String recived via serial pins
@return Struct that contains three different Strings (sn, msg, category)
which we'll use in our code depending on the project needs
*/
Payload payloadSplit(String incomingPayload)
{
   Payload splitted;
   int aux = 0;

   /*
   First we separate the msg from the topic (serialnumber/category)
   */
   while (incomingPayload[aux] != '|')
   {
      splitted.msg += incomingPayload[aux];
      aux++;
   }
   aux++;

   /*
   Now we separete the serial number from the device category
   */
   while (incomingPayload[aux] != '/')
   {
      splitted.sn += incomingPayload[aux];
      aux++;
   }
   aux++;

   /*
   Finally we got the device category
   */
   while (incomingPayload[aux] != '/')
   {
      splitted.category += incomingPayload[aux];
      aux++;
   }

   return splitted;
}