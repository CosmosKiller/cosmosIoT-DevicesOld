#include <stdio.h>

#include "MqttCustomFunc.h"

String topicGeneration(String serialNumber)
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
   else if (type == "MOT")
      topic = serialNumber + "/Motors";

   topic.trim();
   return topic;
}

String payloadCapture(byte* payload, unsigned int length) 
{
   String msg = "";
   for (unsigned int i = 0; i < length; i++)
      msg += (char)payload[i];

   msg.trim();

   return msg;
}

void payloadSerialSend(char* topic, byte* payload, unsigned int length)
{
   char toSend[50] = "";
   String msg = payloadCapture(payload, length);
   msg = msg + '|' + topic;
   msg.toCharArray(toSend, 50);

   Serial.write(toSend);
}

Payload_t payloadSerialSplit(String incomingPayload)
{
   Payload_t splitted;
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