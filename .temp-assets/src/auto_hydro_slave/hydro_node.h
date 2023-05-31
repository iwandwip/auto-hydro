/*
 *  hydro_node.h
 *
 *  hydro node header
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef HYDRO_NODE_H
#define HYDRO_NODE_H

#include "Arduino.h"
#include "hydro_def.h"
#include "hydro_sens.h"
#include <stdint.h>

typedef enum {
        RELAY_UP = 0,
        RELAY_DOWN = 1,
        RELAY_LEFT = 2,
        RELAY_RIGHT = 3,
        RELAY_NUMS = 4,
} relay_index;

typedef enum {
        LED_RED_C = 0,
        LED_GREEN_C = 1,
        BUZZER_C = 2,
        INDC_NUMS = 3,
} indc_index;

typedef struct {
        uint8_t relayPin[RELAY_NUMS];
        uint8_t relayState[RELAY_NUMS];
} relay_act_t;

typedef struct {
        uint8_t indcPin[INDC_NUMS];
        uint8_t indcState[INDC_NUMS];
} indc_act_t;

typedef struct {
        relay_act_t relay;
        indc_act_t indc;
} node_t;

void hydro_node_init(node_t *packet);
void hydro_node_loop(node_t *packet);

void hydro_node_debug(node_t *packet);

#endif  // HYDRO_NODE_H