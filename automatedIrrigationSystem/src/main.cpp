#include <Arduino.h>

#include "DHTesp.h"

#include "CosmosIOT.h"

// Defining and initiating our dht11
DHTesp dht;

// Defining and initiating our devices
Devices_t devices[]{
    {"SKTPc-aaa0000", {18, 0, 0}, LOW},
    {"SNRTh-aaa0001", {19, 0, 0}, LOW},
    {"SNRWl-aaa0000", {34, 0, 0}, LOW},
    {"LSCLc-aaa0000", {21, 22, 17}, LOW}};
const int QUANTITY = sizeof(devices) / sizeof(devices[0]);

// Global variables
int sensVal;
float temp;

// Function declaration
static void myCallback(char *topic, byte *payload, unsigned int length);

void setup()
{
  Serial.begin(115200);
  delay(1000);

  cosmosBegin(QUANTITY, devices);

  dht.setup(devices[1].pin[0], DHTesp::DHT11);
  cosmosWifiSetup();
  cosmosMqttSetup(myCallback);
}

void loop()
{
  cosmosMqttLoop(QUANTITY, devices);

  sensVal = analogRead(devices[2].pin[0]);
  temp = dht.getTemperature();

  if (sensVal == 0)
  {
    digitalWrite(devices[0].pin[0], LOW);
    devices[0].state = LOW;
    digitalWrite(devices[3].pin[1], LOW);

    digitalWrite(devices[3].pin[0], HIGH);
    Serial.print("Main loop | Warning! Water under critical level: ");
    Serial.println(sensVal);
    delay(500);

    digitalWrite(devices[3].pin[0], LOW);
    delay(500);
  }
  else if (temp >= 25.0)
  {
    digitalWrite(devices[0].pin[0], LOW);
    devices[0].state = LOW;
    digitalWrite(devices[3].pin[1], LOW);

    digitalWrite(devices[3].pin[0], HIGH);
    digitalWrite(devices[3].pin[1], HIGH);
    Serial.print("Main loop | Warning! Temperature to high: ");
    Serial.println(temp);
    delay(500);

    digitalWrite(devices[3].pin[0], LOW);
    digitalWrite(devices[3].pin[1], LOW);
    delay(500);
  }
  else
  {
    digitalWrite(devices[3].pin[2], HIGH);
    Serial.println("System ready.");
    Serial.print("Water level OK: ");
    Serial.println(sensVal);
    Serial.print("Temperature OK: ");
    Serial.println(temp);
    delay(500);
  }
}

static void myCallback(char *topic, byte *payload, unsigned int length)
{
  Serial.println("incoming msg");
  delay(1000);

  if (devices[0].state == LOW)
  {
    sensVal = analogRead(devices[2].pin[0]);
    temp = dht.getTemperature();

    if (sensVal > 0 && temp < 25.0)
    {
      digitalWrite(devices[0].pin[0], HIGH);
      devices[0].state = HIGH;

      digitalWrite(devices[3].pin[2], LOW);
      digitalWrite(devices[3].pin[1], HIGH);
      Serial.print("Water level OK: ");
      Serial.println(sensVal);
      Serial.print("Temperature OK: ");
      Serial.println(temp);
      delay(500);
    }
    else if (sensVal == 0)
    {
      digitalWrite(devices[3].pin[0], HIGH);
      Serial.print("Mqtt loop | Warning! Water under critical level: ");
      Serial.println(sensVal);
      delay(500);

      digitalWrite(devices[3].pin[0], LOW);
      delay(500);
    }
    else if (temp >= 25.0)
    {
      digitalWrite(devices[3].pin[0], HIGH);
      digitalWrite(devices[3].pin[1], HIGH);
      Serial.print("Mqtt loop | Warning! Temperature to high: ");
      Serial.println(temp);
      delay(500);

      digitalWrite(devices[3].pin[0], LOW);
      digitalWrite(devices[3].pin[1], LOW);
      delay(500);
    }
  }
  else
  {
    digitalWrite(devices[0].pin[0], LOW);
    devices[0].state = LOW;

    for (int i = 0; i < 3; i++)
    {
      digitalWrite(devices[3].pin[i], LOW);
    }
  }
}