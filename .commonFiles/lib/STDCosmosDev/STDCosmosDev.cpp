#include "STDCosmosDev.h"

const char *socketControll(String snValue, int qty, Devices_t *dev)
{
    String devState;
    String msg;
    char newMsg[40];

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
    msg.toCharArray(newMsg, 40);
    return newMsg;
}

const char *lightControll(String snValue, String rgbValues, int qty, Devices_t *dev)
{
    int j = 0;
    int ledValue[4];
    String devState;
    String msg;
    char newMsg[40];

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
    msg.toCharArray(newMsg, 40);
    return newMsg;
}

void btnMonitor(int *btnArray, int btnQty, Devices_t *dev)
{
    for (int i = 0; i < btnQty; i++)
    {
        if (digitalRead(btnArray[i]) == HIGH)
        {
            String devType = "";
            for (int j = 0; j < 3; j++)
                devType += dev[i].sn;

            if (devType == "SKT")
                socketControll(dev[i].sn, 1, &dev[i]);
            else if (devType == "LSC")
                lightControll(dev[i].sn, "000/000/000/000/", 1, &dev[i]);
        }
    }
}
