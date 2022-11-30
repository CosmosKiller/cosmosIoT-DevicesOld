#include <string.h>

#include "Servo.h"

#include "CosmosClient.h"

// Defining and initiating our the servo
Servo servo;

// Defining btn pins
const int btn1 = 2;
const int btn2 = 3;

// Defining and initiating our devices
Devices_t devices[]{
    {"LSCLw-aaa0000", {0, 0, 0}, LOW},
    {"MOTSv-aaa0000", {1, 0, 0}, LOW}};
const int QUANTITY = sizeof(devices) / sizeof(devices[0]);

// Function declaration
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

  cosmosWifiSetup();
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
   * the strstr() function will return a pointer
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
  cosmosMqttPublish("1", devices[1].sn, RX_STATE);
  cosmosMqttPublish("Sirviendo...", devices[1].sn, RX_CONTROL);

  servo.attach(devices[1].pin[0]);
  delay(100);

  servo.write(deg);
  digitalWrite(devices[0].pin[0], HIGH);
  delay(2000);
  servo.write(0);
  digitalWrite(devices[0].pin[0], LOW);
  delay(100);

  servo.detach();
  delay(100);
  
  cosmosMqttPublish("0", devices[1].sn, RX_STATE);
  cosmosMqttPublish("Cerrado", devices[1].sn, RX_CONTROL);
}