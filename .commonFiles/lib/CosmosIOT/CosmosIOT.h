#include <Arduino.h>
/*
Custom library for all Cosmos-IoT platform projects
*/

/*
Standard structure for devices management
DON'T MODIFY THIS STRUCT!
*/
struct CosmosIOT {
const String sn; //Serial Number of the devices
const int pin[3]; //Pin in which the device will be connected
int state; //Initial state of the device (HIGH/LOW)
};

typedef struct CosmosIOT Devices;

/*
Use this function within the setup() in order
to configure pin modes and initial values.

@param qty Quantity of devices used in the project
@param dev[] Devices strutct that contains all of the info
about the devices used in the project
*/
void cosmosBegin(int qty, Devices dev[])
{
  for (int i = 0; i < qty; i++)
  {
    String devType = "";
    for  (int j = 0 ; j < 3 ; j++)
      devType += dev[i].sn[j];

    if (devType == "SKT")
    {
      pinMode(dev[i].pin[0], OUTPUT);
      digitalWrite(dev[i].pin[0], dev[i].state);
    }
    else if (devType == "LSC")
    {
      analogWrite(dev[i].pin[0], dev[i].state);
      analogWrite(dev[i].pin[1], dev[i].state);
      analogWrite(dev[i].pin[2], dev[i].state);
    }
  }
}

/*
This function is used to controll the state (on/off)
of a certain socket

@param snValue Serial number used to compare and 
determine with which socket we are trying to interact
@param qty Quantity of devices used in the project
@param dev[] Devices strutct that contains all of the info
about the devices used in the project
*/
String socketControll(String snValue, int qty, Devices dev[])
{
  String devState = "";
  String msg = "";

  for (int i = 0 ; i < qty ; i++)
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

/*
This function is used to controll color and brightness of
any RGB ligth source

@param snValue Serial number used to compare and
determine with which ligth source we are trying to interact
@param rgbValues An string that contains Brigthness and RGB values.
Each of them must be in the following order -> "Bright/Rvalue/Gvalue/Bvalue/"
; eg. 050/255/112/017/
@param qty Quantity of devices used in the project
@param dev[] Devices strutct that contains all of the info
about the devices used in the project
*/
String lightControll(String snValue, String rgbValues, int qty, Devices dev[])
{ 
  int j = 0;
  int ledValue[4];
  String devState = "";
  String msg = "";

  for (int i = 0 ; i < 4 ; i++)
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

  for (int i = 0 ; i < qty ; i++)
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

/*
Call this function whenever you need/want to
use buttons to locally control any device.

@param btnArray[] Pins in which the buttons are be connected 
@param devArray[] Serial numbers used to compare and
determine with which devices want to interact
@param btnQty Quantity of buttons used in the project
@param devQty Quantity of devices to be checked
@param dev[] Devices strutct that contains all of the info
about the devices used in the project
*/
void btnMonitor(int btnArray[], String devArray[], int btnQty, int devQty, Devices dev[])
{ 
  for (int i = 0 ; i < btnQty ; i++)
  {
    if (digitalRead(btnArray[i]) == HIGH)
    {
      String devType = "";
      for  (int j = 0 ; j < 3 ; j++)
      devType += devArray[i];

      if (devType == "SKT")
        socketControll(devArray[i], devQty, dev);
      else if (devType == "LSC")
        lightControll(devArray[i], "000/000/000/000/", devQty, dev);  
    }
  }
}