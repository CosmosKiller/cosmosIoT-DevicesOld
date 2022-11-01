#include <stdio.h>

#include "CosmosIOT.h"

void cosmosBegin(int qty, Devices_t dev[])
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

String socketControll(String snValue, int qty, Devices_t dev[])
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
}

String lightControll(String snValue, String rgbValues, int qty, Devices_t dev[])
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

void btnMonitor(int btnArray[], String devArray[], int btnQty, int devQty, Devices_t dev[])
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