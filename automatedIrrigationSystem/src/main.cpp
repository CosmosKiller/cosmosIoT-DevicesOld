#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "CosmosClient.h"
#include "CosmosPumpControl.h"

// Defining and initiating our dht11
TaskHandle_t Task1;

// Defining and initiating our devices
Devices_t devices[]{
    {"SKTPc-aaa0000", {18, 0, 0}, LOW},
    {"SNRTh-aaa0001", {SNRTH_PIN, 0, 0}, LOW},
    {"SNRWl-aaa0000", {SNRWL_PIN, 0, 0}, LOW},
    {"LSCLc-aaa0000", {21, 22, 17}, LOW}};
const int QUANTITY = sizeof(devices) / sizeof(devices[0]);

snrThrPin_t mySensors[]{
    {25, SNRTH_PIN},
    {30, SNRWL_PIN}};

CosmosPump_t myPump{
    devices[0],
    devices[3].pin,
    mySensors[1],
    mySensors[0],
    {0, 0}};

float sensorData[] = {0, 0, 0, 0};

// Global variables
long lastMsg = 0;
char toSend[30];
String msg;

static void myCallback(char *topic, byte *payload, unsigned int length);

void codeForTask1(void *parameter)
{
  for (;;)
  {
    if (devices[0].state == HIGH)
      pumpControl(myPump, ENGAGE_RN, sensorData);
    else
      pumpControl(myPump, ENGAGE_NO, sensorData);

    long now = millis();
    if (now - lastMsg > 2000)
    {
      lastMsg = now;
      msg = String(sensorData[0]);
      msg.toCharArray(toSend, 30);
      cosmosMqttPublish(toSend, devices[2].sn, RX_CONTROL);

      msg = String(sensorData[2]) + "," + String(sensorData[3]);
      msg.toCharArray(toSend, 30);
      cosmosMqttPublish(toSend, devices[1].sn, RX_CONTROL);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  xTaskCreatePinnedToCore(
      codeForTask1,
      "Task1",
      4096,
      NULL,
      1,
      &Task1,
      0);

  cosmosBegin(QUANTITY, devices);

  cosmosWifiSetup();
  cosmosMqttSetup(myCallback);
}

void loop()
{
  cosmosMqttLoop(QUANTITY, devices);
}

static void myCallback(char *topic, byte *payload, unsigned int length)
{
  if (devices[0].state == LOW)
    pumpControl(myPump, ENGAGE_OK, sensorData);
  else
  {
    // Reset pump state
    digitalWrite(devices[0].pin[0], LOW);
    devices[0].state = LOW;

    // Reset pump's LED state
    for (int i = 0; i < 3; i++)
      digitalWrite(devices[3].pin[i], LOW);

    cosmosMqttPublish("0", devices[0].sn, RX_STATE);
    cosmosMqttPublish("Sistema detenido OK!", devices[0].sn, RX_CONTROL);
  }
}