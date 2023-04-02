/*
 *  hydro_sens.h
 *
 *  hydro sens header
 *
 */
 
#pragma once

#ifndef HYDRO_SENS_H
#define HYDRO_SENS_H

#include "Arduino.h"
#include "stdint.h"

#include "NewPing.h"

#define PH_CAL 21.34 + 2.3 - 2.39
#define SCOUNT 30

typedef enum {
        ULTRA_SENS_UP = 0,
        ULTRA_SENS_DOWN = 1,
        ULTRA_SENS_LEFT = 2,
        ULTRA_SENS_RIGHT = 3,
        ULTRA_SENS_NUM = 4,

        // pins cfg
        ULTRA_PINS_TRIGG = 0,
        ULTRA_PINS_ECHO = 1,
        ULTRA_PINS_NUM = 2
} ultrasonic_index_t;

typedef struct {
        NewPing *sensorSonar[ULTRA_SENS_NUM];
        uint8_t sensorTriggPin[ULTRA_SENS_NUM];
        uint8_t sensorEchoPin[ULTRA_SENS_NUM];
        uint16_t rawSensorData[ULTRA_SENS_NUM];
        uint16_t sensorData[ULTRA_SENS_NUM];
        // uint16_t sensorDataPos[ULTRA_SENS_NUM];
        // uint16_t sensorAvg[ULTRA_SENS_NUM];
        // uint32_t sensorTmr[ULTRA_SENS_NUM];
        uint16_t sensorOffset[ULTRA_SENS_NUM];
} ultrasonic_sens_t;

typedef struct {
        float phAct;
        float phVolt;
        uint32_t average;
        int temporary;
        int buffer[10];
} ph_sens_t;

typedef struct {
        // int copyIndex;
        int analogBufferIndex;
        int analogBuffer[SCOUNT];
        int analogBufferTemp[SCOUNT];
        float averageVoltage;
        uint32_t tdsTmr;
        float tdsValue;
} tds_sens_t;

typedef enum {
        ULT_TMR,
        PH_TMR,
        TDS_TMR,
        NUM_TMR
} sens_index;

typedef struct {
        ph_sens_t ph;
        tds_sens_t tds;
        ultrasonic_sens_t ult;
} sens_t;

void hydro_sens_init(sens_t *packet);
void hydro_sens_loop(sens_t *packet);
void hydro_sens_debug(sens_t *packet);

int16_t getUltrasonicVal(sens_t *packet, ultrasonic_index_t c);
float getPhValue(sens_t *packet);
float getTdsValue(sens_t *packet);

#endif  // HYDRO_SENS_H