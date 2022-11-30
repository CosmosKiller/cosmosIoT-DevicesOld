#include "WiFiClient.h"

#include "CosmosClient.h"
#include "secretSerial.h"

#ifdef CIOT_ESP32
#include "WiFiMulti.h"
// #include "analogWrite.h"

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

#ifdef SNRTH_PIN
DHTesp myDHT;
#endif

// MQTT Credentials
const char *mqtt_server = SECRET_MQTT_HOST;
const int mqtt_port = SECRET_MQTT_PORT;
const char *mqtt_user = SECRET_MQTT_USER;
const char *mqtt_pass = SECRET_MQTT_PASS;

WiFiClient espClient;
PubSubClient client(espClient);

/**
 * This functions returns an String that contains the topic and-sub topic that
 * the device is going to suscribe to.
 *
 * @param serialNumber Use the member sn of the "Devices" struct in order to pass the
 * serial number as a paramter
 * @return Notice that 'topic' is returned as an String, you need to
 * cast it to a char array
 */
static String topicGeneration(String serialNumber)
{
  String topic = "";
  String type = "";
  for (int j = 0; j < 14; j++) // 14 is the standar lenght of the serial numbers
  {
    if (j < 3)
      type += serialNumber[j];
  }

  if (type == "HUB")
    topic = serialNumber + "/Homehubs";
  else if (type == "LSC")
    topic = serialNumber + "/Lights";
  else if (type == "SKT")
    topic = serialNumber + "/Sockets";
  else if (type == "SNR")
    topic = serialNumber + "/Sensors";
  else if (type == "CAM")
    topic = serialNumber + "/Cameras";
  else if (type == "MOT")
    topic = serialNumber + "/Motors";

  topic.trim();
  return topic;
}

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

#ifdef SNRTH_PIN
    myDHT.setup(SNRTH_PIN, DHTesp::DHT11);
#endif
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

void cosmosMqttPublish(const char *msg, String devSerial, const char *topic)
{
  String s_topic;
  char auxTopic[40];

  s_topic = devSerial + topic;
  s_topic.toCharArray(auxTopic, 40);

  client.publish(auxTopic, msg);
}