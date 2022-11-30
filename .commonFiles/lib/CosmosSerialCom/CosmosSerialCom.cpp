#include <stdio.h>

#include "CosmosSerialCom.h"

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