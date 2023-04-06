/*
 *  hydro_hw.h
 *
 *  hydro hardware
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef HYDRO_HW_H
#define HYDRO_HW_H

#include "Arduino.h"
#include "stdint.h"
#include "hydro_node.h"
#include "hydro_sens.h"

typedef struct {
        char rxBuffer[250];
        uint8_t rxBufferPtr = 0;
        String data;
} buffer_t;

void hydro_hw_init(void);
void hydro_hw_loop(sens_t* packet_sens, node_t* packet_node);

#endif  // HYDRO_HW_H