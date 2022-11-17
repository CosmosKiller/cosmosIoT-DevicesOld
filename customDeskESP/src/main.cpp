#include "ESP8266WiFi.h"

#include "CosmosIOT.h"
#include "secretSerial.h"

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

//WiFi credentials
const char* ssid = SECRET_SSID;
const char* pass = SECRET_PASS;

//Variables
String incomingStr = "";
Payload_t splitPayload;

//Function declaration
void setup_wifi();

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(15);
  delay(1500);
  randomSeed(micros());
  setup_wifi();

  cosmosMqttSetup(payloadSerialSend);
}

void loop()
{
  cosmosMqttLoop(QUANTITY, devices);
}


//Functions definition
void setup_wifi()
{
  delay(10);
  //Wifi Connection
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
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

    auxTopic = splitPayload.sn + '/' + splitPayload.category;
    auxTopic.toCharArray(topic, 50);
    splitPayload.msg.toCharArray(msg, 50);

    cosmosMqttPublish(topic, msg);
  }
}