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
long lastMsg = 0;
int sensVal;
float temp, hum;
char toSend[30];
String msg;

// Function declaration
static void powerOff(void);
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
  hum = dht.getHumidity();

  if (sensVal == 0)
  {
    powerOff();
    digitalWrite(devices[3].pin[0], HIGH);
    delay(500);
    digitalWrite(devices[3].pin[0], LOW);
    delay(500);

    cosmosMqttPublish("0", devices[0].sn, RX_STATE);
    cosmosMqttPublish("Atención! Nivel de agua crítico", devices[0].sn, RX_CONTROL);
  }
  else if (temp >= 25.0)
  {
    powerOff();
    digitalWrite(devices[3].pin[0], HIGH);
    digitalWrite(devices[3].pin[1], HIGH);
    delay(500);
    digitalWrite(devices[3].pin[0], LOW);
    digitalWrite(devices[3].pin[1], LOW);
    delay(500);

    cosmosMqttPublish("0", devices[0].sn, RX_STATE);
    cosmosMqttPublish("Atención! Nivel de temperatura crítico", devices[0].sn, RX_CONTROL);
  }
  else
  {
    digitalWrite(devices[3].pin[2], HIGH);
    delay(500);

    cosmosMqttPublish("Sistema en espera OK!", devices[0].sn, RX_CONTROL);
  }

  long now = millis();
  if (now - lastMsg > 2000)
  {
    
    msg = String(sensVal);
    msg.toCharArray(toSend, 30);
    cosmosMqttPublish(toSend, devices[2].sn, RX_CONTROL);

    if (isnan(temp) == 0 || isnan(hum) == 0)
    {
    msg = String(temp) + "," + String(hum);
    msg.toCharArray(toSend, 30);
    cosmosMqttPublish(toSend, devices[1].sn, RX_CONTROL);
    }
  }
}

static void powerOff()
{
  digitalWrite(devices[0].pin[0], LOW);
  devices[0].state = LOW;

  for (int i = 0; i < 3; i++)
  {
    digitalWrite(devices[3].pin[i], LOW);
  }
}

static void myCallback(char *topic, byte *payload, unsigned int length)
{
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
      digitalWrite(devices[3].pin[0], LOW);
      delay(500);

      cosmosMqttPublish("1", devices[0].sn, RX_STATE);
    }
    else if (sensVal == 0)
    {
      powerOff();
      digitalWrite(devices[3].pin[0], HIGH);
      delay(500);
      digitalWrite(devices[3].pin[0], LOW);
      delay(500);

      cosmosMqttPublish("0", devices[0].sn, RX_STATE);
    }
    else if (temp >= 25.0)
    {
      powerOff();
      digitalWrite(devices[3].pin[0], HIGH);
      digitalWrite(devices[3].pin[1], HIGH);
      delay(500);
      digitalWrite(devices[3].pin[0], LOW);
      digitalWrite(devices[3].pin[1], LOW);
      delay(500);

      cosmosMqttPublish("0", devices[0].sn, RX_STATE);
    }
  }
  else
  {
    powerOff();

    cosmosMqttPublish("0", devices[0].sn, RX_STATE);
  }
}