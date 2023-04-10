/*
 *  hydro_hw_m.h
 *
 *  hydro hardware master header
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef HYDRO_HW_H
#define HYDRO_HW_H

#include "Arduino.h"
#include "stdint.h"
// #include <vector>

typedef enum {
        ULTRA_SENS_UP = 0,
        ULTRA_SENS_DOWN = 1,
        ULTRA_SENS_LEFT = 2,
        ULTRA_SENS_RIGHT = 3,
        ULTRA_SENS_NUM = 4,

        PH_SENS_ACT = 0,
        TDS_SENS_ACT = 1,
        ETC_SENS_NUM = 2,

        RELAY_UP = 0,
        RELAY_DOWN = 1,
        RELAY_LEFT = 2,
        RELAY_RIGHT = 3,
        RELAY_NUMS = 4
} data_index;

typedef struct {
        float etc_data[ETC_SENS_NUM];
        uint16_t ult_data[ULTRA_SENS_NUM];
        uint8_t state[RELAY_NUMS];
} data_t;

typedef struct {
        char rxBuffer[250];
        uint8_t rxBufferPtr = 0;
        String data;
} buffer_t;

void hydro_hw_init(void);
void hydro_hw_loop(data_t* packet);

#endif  // HYDRO_HW_H