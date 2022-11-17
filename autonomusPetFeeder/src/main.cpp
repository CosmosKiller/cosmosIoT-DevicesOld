#include <string.h>

#include "ESP8266_ISR_Servo.h"
#include "ESP8266WiFi.h"

#include "CosmosIOT.h"
#include "secretSerial.h"

//Defining btn pins
const int btn1 = 4;
const int btn2 = 5;

//Defining and initiating our devices
Devices_t devices []{
  {"LSCLw-aaa0000", {12, 0, 0}, LOW},
  {"MOTSv-aaa0000", {3, 0, 0}, LOW}
};
const int QUANTITY = sizeof(devices)/sizeof(devices[0]);

//Defining and initiating our the servo
ESP8266_ISR_Servo servo;
int8_t srv1_id = servo.setupServo(devices[1].pin[0]);

//WiFi credentials
const char* ssid = SECRET_SSID;
const char* pass = SECRET_PASS;

//Function declaration
void setup_wifi();
static void servoControl(int deg);
static void myCallback(char *topic, byte *payload, unsigned int length);

void setup()
{
  Serial.begin(9600);
  servo.enable(srv1_id);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(devices[0].pin[0], OUTPUT);
  servo.setPosition(srv1_id, 180);
  delay(1000);
  servo.setPosition(srv1_id, 0);
  delay(1000);
  servo.disable(srv1_id);

  cosmosMqttSetup(myCallback);
}

void loop()
{
  cosmosMqttLoop(QUANTITY, devices);
}

static void myCallback(char *topic, byte *payload, unsigned int length)
{
  /*
  * If the topic contains our desire category
  * the strste() function will return a pointer
  * to the first occurrence in it, and store it
  * in the 'category' variable
  */
  char *category = strstr(topic, "Motors");

  if (category != NULL)
  {
    servoControl(90);
  }
}

static void servoControl(int deg)
{
  char auxTopic[30] ="";

  String topic = topicGeneration(devices[1].sn);
  topic.toCharArray(auxTopic, 30);

  servo.enable(srv1_id);
  delay(100);
  servo.setPosition(srv1_id, deg);
  digitalWrite(devices[0].pin[0],HIGH);
  cosmosMqttPublish(auxTopic, "Servo activado");
  delay(2000);
  servo.setPosition(srv1_id, 0);
  digitalWrite(devices[0].pin[0], LOW);
  cosmosMqttPublish(auxTopic, "Servo desactivado");
  delay(1000);
  servo.disable(srv1_id);
  delay(100);
}