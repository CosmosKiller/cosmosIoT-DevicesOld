/**
* The source code used to manage all the I/O functionalities, reciving 
* the commands from the ESP8266 via the serial pins (TX0/RX0)
* It can also be used to return information such as temperature, humidity and
* polution messurements
*/ 

#include "DHT.h"

#include "CosmosClient.h"
#include "CosmosSerialCom.h"
#include "STDCosmosDev.h"

//Defining and initiating our devices
Devices_t devices []{
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

//Defining and initiating the DHT22 Sensor
#define DHTPIN devices[9].pin[0]
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

//Defining btn pins
int btnArr[] = {22, 24, 26, 28, 30, 32, 34, 36};
int btnArrLen = sizeof(btnArr)/sizeof(btnArr[0]);

/**
* Defining the array that we'll use to
* asign a btn to a certain device.
*/
String btnToDev[] = {
  devices[0].sn,
  devices[1].sn,
  devices[2].sn,
  devices[3].sn,
  devices[4].sn,
  devices[5].sn,
  devices[6].sn,
  devices[7].sn
};

//Variables
long lastMsg = 0;
float humudity, temperature;
bool valueComplete = false;
char toSend[50] = "";
String incomingStr = "";
Payload_t splitPayload;


void setup()
{
  Serial1.begin(115200);
  Serial1.setTimeout(15);
  Serial.begin(115200);
  dht.begin();
  cosmosBegin(QUANTITY, devices);
}

void loop()
{ 
  humudity = dht.readHumidity();
  temperature = dht.readTemperature();

  if (splitPayload.category != "")
  {
    if (splitPayload.category == "Sockets")
    {
      String msg = socketControll(splitPayload.sn, QUANTITY, devices);
      msg.toCharArray(toSend, 50);
      Serial1.write(toSend);
    }
    else if (splitPayload.category == "Lights")
    {
      String msg = lightControll(splitPayload.sn, splitPayload.msg, QUANTITY, devices);
      msg.toCharArray(toSend, 50);
      Serial1.write(toSend);
    }
  }

  long now = millis();
  if (now - lastMsg > 2000)
  {
    lastMsg = now;
    String msg = String(temperature) + "," + String(humudity) + "|" + devices[9].sn + RX_CONTROL;
    msg.toCharArray(toSend, 50);

    Serial1.write(toSend);
  }

  btnMonitor(btnArr, btnToDev, btnArrLen, QUANTITY, devices);

  incomingStr = "";
  splitPayload = {};
}

void serialEvent1()
{ 
  while (Serial1.available() > 0)
  {
    incomingStr = Serial1.readString();
    incomingStr = incomingStr + "/";
    splitPayload = payloadSerialSplit(incomingStr);
  }
}