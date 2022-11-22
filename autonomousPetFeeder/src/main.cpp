#include <string.h>

#include "Servo.h"

#include "CosmosIOT.h"

//Defining btn pins
const int btn1 = 4;
const int btn2 = 5;

//Defining and initiating our devices
Devices_t devices []{
  {"LSCLw-aaa0000", {0, 0, 0}, LOW},
  {"MOTSv-aaa0000", {1, 0, 0}, LOW}
};
const int QUANTITY = sizeof(devices)/sizeof(devices[0]);

//Defining and initiating our the servo
Servo servo;

//Function declaration
static void servoControl(int deg);
static void myCallback(char *topic, byte *payload, unsigned int length);

void setup()
{
  Serial.begin(115200);
  servo.attach(devices[1].pin[0]);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(devices[0].pin[0], OUTPUT);
  servo.write(180);
  delay(1000);
  servo.write(0);
  delay(1000);
  servo.detach();

  cosmosMqttSetup(myCallback, CIOT_ESP8266); 
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

  category = NULL;
}

static void servoControl(int deg)
{
  char auxTopic[30] ="";

  String topic = devices[1].sn + "/rx_state";
  topic.toCharArray(auxTopic, 30);

  servo.attach(devices[1].pin[0]);
  delay(100);
  servo.write(deg);
  digitalWrite(devices[0].pin[0],HIGH);
  cosmosMqttPublish(auxTopic, "1");
  delay(2000);
  servo.write(0);
  digitalWrite(devices[0].pin[0], LOW);
  cosmosMqttPublish(auxTopic, "0");
  delay(1000);
  servo.detach();
  delay(100);
}