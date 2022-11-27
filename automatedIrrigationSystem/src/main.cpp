#include "DHTesp.h"

#include "CosmosIOT.h"

#define ENGAGE_NO 0
#define ENGAGE_OK 1
#define ENGAGE_RN 2

// Defining and initiating our dht11
DHTesp dht;
TaskHandle_t Task1;

// Defining and initiating our devices
Devices_t devices[]{
    {"SKTPc-aaa0000", {18, 0, 0}, LOW},
    {"SNRTh-aaa0001", {19, 0, 0}, LOW},
    {"SNRWl-aaa0000", {34, 0, 0}, LOW},
    {"LSCLc-aaa0000", {21, 22, 17}, LOW}};
const int QUANTITY = sizeof(devices) / sizeof(devices[0]);

// Global variables
long lastMsg = 0;
int wl;
float temp, hum;
char toSend[30];
String msg;

// Function declaration

/**
 * @brief Simple loop for reseting the LED state
 */
static void powerOff(void);

/**
 * @brief Call this function whenever you want to check the
 * status of the pump
 *
 * @param engage If you want to turn on the pump ENGAGE_OK ;
 * If you want to leave it in idle ENGAGE_NO ;
 * If pump is already running ENGAGE_RN
 * @param wlThreshold Minimum water level to get the pump started ;
 * expresed in %
 * @param tempThreshold Maximum aceptable temperature to get the pump
 * started
 */
static void pumpControl(int engage, int wlThreshold, float tempThreshold);

static void myCallback(char *topic, byte *payload, unsigned int length);

void codeForTask1(void *parameter)
{
  for (;;)
  {
    if (devices[0].state == HIGH)
      pumpControl(ENGAGE_RN, 25, 30.0);
    else
      pumpControl(ENGAGE_NO, 25, 30.0);

    long now = millis();
    if (now - lastMsg > 2000)
    {
      lastMsg = now;
      msg = String(wl);
      msg.toCharArray(toSend, 30);
      cosmosMqttPublish(toSend, devices[2].sn, RX_CONTROL);

      msg = String(temp) + "," + String(hum);
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
  dht.setup(devices[1].pin[0], DHTesp::DHT11);
  cosmosWifiSetup();
  cosmosMqttSetup(myCallback);
}

void loop()
{
  cosmosMqttLoop(QUANTITY, devices);
}

static void powerOff()
{
  digitalWrite(devices[0].pin[0], LOW);
  devices[0].state = LOW;

  for (int i = 0; i < 3; i++)
    digitalWrite(devices[3].pin[i], LOW);
}

static void pumpControl(int engage, int wlThreshold, float tempThreshold)
{
  wl = analogRead(devices[2].pin[0]);
  temp = dht.getTemperature();
  hum = dht.getHumidity();

  if (isnan(temp) == 0 && isnan(hum) == 0 && isnan(wl) == 0)
  {
    if (wl < wlThreshold)
    {
      powerOff();
      digitalWrite(devices[3].pin[0], HIGH);
      delay(1000);
      digitalWrite(devices[3].pin[0], LOW);
      delay(1000);

      cosmosMqttPublish("0", devices[0].sn, RX_STATE);
      cosmosMqttPublish("Atención! Nivel de agua crítico", devices[0].sn, RX_CONTROL);
    }
    else if (temp >= tempThreshold)
    {
      powerOff();
      digitalWrite(devices[3].pin[0], HIGH);
      digitalWrite(devices[3].pin[1], HIGH);
      delay(1000);
      digitalWrite(devices[3].pin[0], LOW);
      digitalWrite(devices[3].pin[1], LOW);
      delay(1000);

      cosmosMqttPublish("0", devices[0].sn, RX_STATE);
      cosmosMqttPublish("Atención! Nivel de temperatura crítico", devices[0].sn, RX_CONTROL);
    }
    else if (wl > wlThreshold && temp < tempThreshold)
    {
      switch (engage)
      {
      case ENGAGE_OK:
        digitalWrite(devices[0].pin[0], HIGH);
        devices[0].state = HIGH;

        digitalWrite(devices[3].pin[2], LOW);
        digitalWrite(devices[3].pin[1], HIGH);
        digitalWrite(devices[3].pin[0], LOW);
        delay(2000);

        cosmosMqttPublish("1", devices[0].sn, RX_STATE);
        cosmosMqttPublish("Sistema iniciado...", devices[0].sn, RX_CONTROL);
        break;

      case ENGAGE_NO:
        digitalWrite(devices[3].pin[2], HIGH);
        delay(2000);

        cosmosMqttPublish("Sistema en espera.", devices[0].sn, RX_CONTROL);
        break;

      case ENGAGE_RN:
        digitalWrite(devices[3].pin[1], HIGH);
        delay(2000);
        cosmosMqttPublish("Sistema en marcha OK!", devices[0].sn, RX_CONTROL);

      default:
        break;
      }
    }
  }
}

static void myCallback(char *topic, byte *payload, unsigned int length)
{
  if (devices[0].state == LOW)
    pumpControl(ENGAGE_OK, 25, 30.0);
  else
  {
    powerOff();
    cosmosMqttPublish("0", devices[0].sn, RX_STATE);
    cosmosMqttPublish("Sistema detenido OK!", devices[0].sn, RX_CONTROL);
  }
}