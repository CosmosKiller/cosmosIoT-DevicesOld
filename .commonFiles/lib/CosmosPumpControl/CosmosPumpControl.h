/**
 * Custom library for std CosmosIOT devices mangement
 */

#ifndef MAIN_COSMOSPUMPCONTROL_H_
#define MAIN_COSMOSPUMPCONTROL_H_

#include "CosmosClient.h"

#define ENGAGE_NO 0
#define ENGAGE_OK 1
#define ENGAGE_RN 2

/**
 * @brief Use this scruct within the
 * CosmosPump_t to store the parameters
 * of the sensors
 * 
 */
typedef struct snrThrPin
{
    /* Threshold Level*/
    const int Thr;
    /* Pin in which the sensor is connected*/
    const int Pin;
} snrThrPin_t;

/**
 * @brief Use this struct to define all the parameters
 * that needs to be checked in order to get the pump
 * going
 *
 * @note If you are going to do without any of the sensors,
 * just create an arry of zeros -> {0, 0}
 */
typedef struct CosmosPump
{
    /* Pump relay serial number, pins and state */
    Devices_t pump;
    /* Pins in which the pump's LED is connected */
    const int *ledPins;
    /* Water level sensor */
    snrThrPin_t snrWl;
    /* Temperature sensor */
    snrThrPin_t snrTh;
    /* Soil moisture sensor */
    snrThrPin_t snrSm;
} CosmosPump_t;

/**
 * @brief Struct in which the sensed data will be stored
 * Values as indexed as following -> 
 * 0 = wlData
 * 1 = smData
 * 2 = tmData
 * 3 = hmData
 */
typedef struct snrData
{
    int wlData;
    int smData;
    float tmData;
    float hmData;
} snrData_t;

/**
 * @brief Call this function whenever you want to check the
 * status of the pump
 *
 * @param pumpID Struct that defines the pump
 * @param engage If you want to turn on the pump ENGAGE_OK ;
 * If you want to leave it in idle ENGAGE_NO ;
 * If pump is already running ENGAGE_RN
 * @param sensorData Struct in which the sensed data will be stored
 */
void pumpControl(CosmosPump_t pumpID, int engage, snrData_t *sensorData);

#endif /* MAIN_COSMOSPUMPCONTROL_H_ */