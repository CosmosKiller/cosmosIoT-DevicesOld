#include <stdio.h>

#include "WiFiClient.h"

#include "CosmosIOT.h"
#include "secretSerial.h"

#ifdef CIOT_ESP32
#include "WiFiMulti.h"
#include "analogWrite.h"

// WiFi Credentials
const char *ssid = SECRET_SSID;
const char *pass = SECRET_PASS;

WiFiMulti myWiFi;

void cosmosWifiSetup(void)
{
  delay(10);
  // ESP2866 Wifi Connection
  myWiFi.addAP(ssid, pass);

  while (myWiFi.run() != WL_CONNECTED)
  {
    delay(500);
  }
}
#endif

#ifdef CIOT_ESP8266
#include "ESP8266WiFi.h"

// WiFi Credentials
const char *ssid = SECRET_SSID;
const char *pass = SECRET_PASS;

void cosmosWifiSetup(void)
{
  delay(10);
  // ESP2866 Wifi Connection
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
}
#endif

// MQTT Credentials
const char *mqtt_server = SECRET_MQTT_HOST;
const int mqtt_port = SECRET_MQTT_PORT;
const char *mqtt_user = SECRET_MQTT_USER;
const char *mqtt_pass = SECRET_MQTT_PASS;

WiFiClient espClient;
PubSubClient client(espClient);


/**
 * @brief Client creation, connection and dinamic topic subscription
 *
 * @param qty Quantity of devices used in the project
 * @param dev[] Devices strutct that contains all of the info
 * about the devices used in the project
 */
static void cosmosMqttReconect(int qty, Devices_t dev[])
{
  while (!client.connected())
  {
    // Client creation
    String clientId = "esp_";
    clientId += String(random(0xffff), HEX);
    // Coneccting the client
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass))
    {
      // Dinamic subscription based on the device types

      for (int j = 0; j < qty; j++)
      {
        char auxTopic[30] = "";

        String topic = topicGeneration(dev[j].sn);
        topic.toCharArray(auxTopic, 30);
        if (topic.length() > 14)
          client.subscribe(auxTopic);
      }
    }
    else
    {
      delay(5000);
    }
  }
}

void cosmosBegin(int qty, Devices_t dev[])
{
  for (int i = 0; i < qty; i++)
  {
    String devType = "";
    for (int j = 0; j < 3; j++)
      devType += dev[i].sn[j];

    if (devType == "SKT")
    {
      pinMode(dev[i].pin[0], OUTPUT);
      digitalWrite(dev[i].pin[0], dev[i].state);
    }
    else if (devType == "LSC")
    {
      #ifdef CIOT_ESP32
      pinMode(dev[i].pin[0], OUTPUT);
      pinMode(dev[i].pin[1], OUTPUT);
      pinMode(dev[i].pin[2], OUTPUT);
      digitalWrite(dev[i].pin[0], dev[i].state);
      digitalWrite(dev[i].pin[1], dev[i].state);
      digitalWrite(dev[i].pin[2], dev[i].state);
      #endif

      #ifdef CIOT_ESP8266
      analogWrite(dev[i].pin[0], dev[i].state);
      analogWrite(dev[i].pin[1], dev[i].state);
      analogWrite(dev[i].pin[2], dev[i].state);
      #endif
    }
  }
}

String socketControll(String snValue, int qty, Devices_t dev[])
{
  String devState = "";
  String msg = "";

  for (int i = 0; i < qty; i++)
  {
    if (snValue == dev[i].sn)
    {
      if (dev[i].state == HIGH)
      {
        digitalWrite(dev[i].pin[0], LOW);
        dev[i].state = LOW;
        devState = "1";
      }
      else
      {
        digitalWrite(dev[i].pin[0], HIGH);
        dev[i].state = HIGH;
        devState = "0";
      }
    }
  }

  msg = devState + "|" + snValue + "/rx_state";
  return msg;
}

String lightControll(String snValue, String rgbValues, int qty, Devices_t dev[])
{
  int j = 0;
  int ledValue[4];
  String devState = "";
  String msg = "";

  for (int i = 0; i < 4; i++)
  {
    String auxValue = "";
    while (rgbValues[j] != '/')
    {
      auxValue += rgbValues[j];
      j++;
    }
    auxValue.trim();
    ledValue[i] = auxValue.toInt();
    j++;
  }

  for (int i = 0; i < qty; i++)
  {
    int rValue = 0;
    int gValue = 0;
    int bValue = 0;

    if (snValue == dev[i].sn)
    {
      rValue = map(ledValue[1], 0, 100, 0, ledValue[0]);
      gValue = map(ledValue[2], 0, 100, 0, ledValue[0]);
      bValue = map(ledValue[3], 0, 100, 0, ledValue[0]);

      analogWrite(dev[i].pin[0], rValue);
      analogWrite(dev[i].pin[1], gValue);
      analogWrite(dev[i].pin[2], bValue);
    }
  }

  if (rgbValues == "000/000/000/000/")
    devState = "0";
  else
    devState = "1";

  msg = devState + "|" + snValue + "/rx_state";
  return msg;
}

void btnMonitor(int btnArray[], String devArray[], int btnQty, int devQty, Devices_t dev[])
{
  for (int i = 0; i < btnQty; i++)
  {
    if (digitalRead(btnArray[i]) == HIGH)
    {
      String devType = "";
      for (int j = 0; j < 3; j++)
        devType += devArray[i];

      if (devType == "SKT")
        socketControll(devArray[i], devQty, dev);
      else if (devType == "LSC")
        lightControll(devArray[i], "000/000/000/000/", devQty, dev);
    }
  }
}

void cosmosMqttLoop(int qty, Devices_t dev[])
{
  if (!client.connected())
  {
    cosmosMqttReconect(qty, dev);
  }
  client.loop();
}

void cosmosMqttSetup(MQTT_CALLBACK_SIGNATURE)
{
  // mqtt client setup
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void cosmosMqttPublish(const char *topic, const char *msg)
{
  client.publish(topic, msg);
}