#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <MqttCustomFunc.h>
#include <CosmosIOT.h>
#include "secretSerial.h"

//Defining and initiating our devices
Devices devices []{
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

//MQTT Credentials
const char* mqtt_server = SECRET_MQTT_HOST;
const int mqtt_port = SECRET_MQTT_PORT;
const char* mqtt_user = SECRET_MQTT_USER;
const char* mqtt_pass = SECRET_MQTT_PASS;

WiFiClient espClient;
PubSubClient client(espClient);

//Variables
String incomingStr = "";
Payload splitPayload;

//Function declaration
void setup_wifi();
void mqtt_reconect();
void response();

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(15);
  delay(1500);
  randomSeed(micros());
  setup_wifi();

  //mqtt client setup
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(payloadSerialSend);
}

void loop()
{
  if (!client.connected())
  {
    mqtt_reconect();
  }
  client.loop();
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

void mqtt_reconect()
{
  while (!client.connected())
  {
    //Client creation
    String clientId = "esp_";
    clientId += String(random(0xffff), HEX);
    //Coneccting the client
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass))
    {
      //Dinamic subscription based on the device types
      int aux = 0;
    
      for (int j = 0; j < QUANTITY; j++)
      {
        char auxTopic[30] ="";

        String topic = topicGeneration(devices[j].sn, aux);
      	topic.toCharArray(auxTopic, 30);
   		  if (topic.length() > 14)
          client.subscribe(auxTopic);
      
        aux += 14;
      }
    }
    else
    {
      delay(5000);
    }
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
    splitPayload = payloadSplit(incomingStr);

    auxTopic = splitPayload.sn + '/' + splitPayload.category;
    auxTopic.toCharArray(topic, 50);
    splitPayload.msg.toCharArray(msg, 50);

    client.publish(topic, msg);
  }
}