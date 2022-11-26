#include "CosmosIOT.h"

//Defining and initiating our devices
Devices_t devices []{
  {"SKTMg-aaa0000", {37, 0, 0}, HIGH},
  {"SKTMg-aaa0001", {35, 0, 0}, HIGH},
  {"SKTMg-aaa0002", {33, 0, 0}, HIGH},
  {"SKTMg-aaa0003", {31, 0, 0}, HIGH},
  {"SKTMg-aaa0004", {29, 0, 0}, HIGH},
  {"SKTMg-aaa0005", {27, 0, 0}, HIGH},
  {"SKTMg-aaa0006", {25, 0, 0}, HIGH},
  {"SKTMg-aaa0007", {23, 0, 0}, HIGH},
  {"LSCSc-aaa0000", {4, 3, 2}, LOW},
  {"SNRTh-aaa0000", {39, 0, 0}, HIGH}
};

const int QUANTITY = sizeof(devices)/sizeof(devices[0]);

//Variables
String incomingStr = "";
Payload_t splitPayload;

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(15);
  delay(1500);
  randomSeed(micros());

  cosmosMqttSetup(payloadSerialSend);
}

void loop()
{
  cosmosMqttLoop(QUANTITY, devices);
}

void serialEvent()
{
  String auxTopic = "";
  char topic[50] = "";
  char msg[50] = "";

  while (Serial.available() > 0)
  {
    incomingStr = Serial.readString();
    incomingStr = incomingStr + "/";
    splitPayload = payloadSerialSplit(incomingStr);

    auxTopic = '/' + splitPayload.category;
    auxTopic.toCharArray(topic, 50);
    splitPayload.msg.toCharArray(msg, 50);

    cosmosMqttPublish(msg, splitPayload.sn, topic);
  }
}