#include "CosmosPumpControl.h"

/**
 * @brief Simple loop for reseting the pump state/values
 */
static void powerOff(CosmosPump_t *pumpID)
{
    // Reset pump state
    digitalWrite(pumpID->pump->pin[0], LOW);
    pumpID->pump->state = LOW;

    // Reset pump's LED state
    for (int i = 0; i < 3; i++)
        digitalWrite(pumpID->ledPins[i], LOW);
}

void pumpControl(CosmosPump_t *pumpID, int engage, snrData_t *sensedData)
{
    int wl = 0;
    int sm = 0;
    float temp = 0;
    float hum = 0;

#ifdef SNRWL_PIN
    wl = analogRead(pumpID->snrWl.Pin);
    wl = map(wl, 0, 4095, 0, 100);
#endif

#ifdef SNRSM_PIN
    sm = analogRead(pumpID->snrSm.Pin);
    sm = map(sm, 0, 4095, 0, 100);
#endif

#ifdef SNRTH_PIN
    temp = myDHT.getTemperature();
    hum = myDHT.getHumidity();
#endif

    if (wl < pumpID->snrWl.Thr && !isnan(wl))
    {
        powerOff(pumpID);
        cosmosMqttPublish("0", pumpID->pump->sn, RX_STATE);
        cosmosMqttPublish("Atención! Nivel de agua crítico", pumpID->pump->sn, RX_CONTROL);
        digitalWrite(pumpID->ledPins[0], HIGH);
        delay(1000);
        digitalWrite(pumpID->ledPins[0], LOW);
        delay(1000);
    }
    else if (sm < pumpID->snrSm.Thr && !isnan(sm))
    {
        powerOff(pumpID);
        cosmosMqttPublish("0", pumpID->pump->sn, RX_STATE);
        cosmosMqttPublish("Atención! Nivel de humedad del suelo crítico", pumpID->pump->sn, RX_CONTROL);
        digitalWrite(pumpID->ledPins[0], HIGH);
        digitalWrite(pumpID->ledPins[2], HIGH);
        delay(1000);
        digitalWrite(pumpID->ledPins[0], LOW);
        digitalWrite(pumpID->ledPins[2], LOW);
        delay(1000);
    }
    else if (temp > pumpID->snrTh.Thr && !isnan(temp))
    {
        powerOff(pumpID);
        cosmosMqttPublish("0", pumpID->pump->sn, RX_STATE);
        cosmosMqttPublish("Atención! Nivel de temperatura crítico", pumpID->pump->sn, RX_CONTROL);
        digitalWrite(pumpID->ledPins[0], HIGH);
        digitalWrite(pumpID->ledPins[1], HIGH);
        delay(1000);
        digitalWrite(pumpID->ledPins[0], LOW);
        digitalWrite(pumpID->ledPins[1], LOW);
        delay(1000);
    }
    else if (wl >= pumpID->snrWl.Thr && temp <= pumpID->snrTh.Thr && sm >= pumpID->snrSm.Thr)
    {
        switch (engage)
        {
        case ENGAGE_OK:
            digitalWrite(pumpID->pump->pin[0], HIGH);
            pumpID->pump->state = HIGH;
            cosmosMqttPublish("1", pumpID->pump->sn, RX_STATE);
            cosmosMqttPublish("Sistema iniciado...", pumpID->pump->sn, RX_CONTROL);
            digitalWrite(pumpID->ledPins[2], LOW);
            digitalWrite(pumpID->ledPins[1], HIGH);
            digitalWrite(pumpID->ledPins[0], LOW);
            delay(2000);

            break;

        case ENGAGE_NO:
            digitalWrite(pumpID->ledPins[2], HIGH);
            cosmosMqttPublish("Sistema en espera.", pumpID->pump->sn, RX_CONTROL);
            delay(2000);
            break;

        case ENGAGE_RN:
            digitalWrite(pumpID->ledPins[1], HIGH);
            cosmosMqttPublish("Sistema en marcha OK!", pumpID->pump->sn, RX_CONTROL);
            delay(2000);
            break;

        default:
            break;
        }
    }
    sensedData->wlData = wl;
    sensedData->smData = sm;
    sensedData->tmData = temp;
    sensedData->hmData = hum;
}
